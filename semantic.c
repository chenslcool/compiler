#include "semantic.h"
#include "tree.h"
#include <assert.h>
#include <string.h>
int numHashSearch = 0;
int hashType = 1;
struct Variable *variableTable[TABLE_SIZE];
struct Structure *structureTable[TABLE_SIZE];
struct Func *funcTable[TABLE_SIZE];
struct InterCode* InterCodeList = NULL;

struct Type * intTypePtr = NULL;

//中间代码中临时变量数量
int nrTmpVar = 0;
//中间代码中跳转标号数量
int nrLabel = 0;

int calculateWidth(struct Type* typePtr){
    assert(typePtr != NULL);
    //不用到width属性
    int sumWidth = 0;
    switch (typePtr->kind)
    {
    case BASIC:{
        sumWidth = INT_FLOAT_SIZE;
    }break;
    case ARRAY:{
        //计算维数乘积
        int product = 1;
        while (typePtr->kind == ARRAY)
        {
            product = product * typePtr->info.array->numElem;
            typePtr = typePtr->info.array->elem;
        }
        sumWidth = product * calculateWidth(typePtr);
    }break;
    case STRUCTURE:{
        struct FieldList * ptr = typePtr->info.structure->structureInfo;
        while (ptr != NULL)
        {
            sumWidth += calculateWidth(ptr->type);
            ptr = ptr->next;
        }
    }break;
    }
    return sumWidth;
}

void reverseArray(struct Type * arrayTypePtr){
    assert(arrayTypePtr != NULL);
    assert(arrayTypePtr->kind == ARRAY);
    //先计算总维数
    int numDim = 0;
    struct Type * curType = arrayTypePtr;
    while ((curType != NULL) && (curType->kind == ARRAY))
    {
        numDim++;
        curType = curType->info.array->elem;
    }
    assert(curType != NULL);
    int baseWidth = calculateWidth(curType);//基类型宽度
    if(numDim == 1){ // 一维数组
        arrayTypePtr->info.array->elemWidth = baseWidth;
        return;
    }
    //高维
    int* dimSizes = (int*)malloc(sizeof(int)*numDim);
    int* dimWidths = (int*)malloc(sizeof(int)*numDim);
    int curDim = numDim - 1;
    curType = arrayTypePtr;
    while (curType->kind == ARRAY)
    {
        dimSizes[curDim] = curType->info.array->numElem;
        curDim--;
        curType = curType->info.array->elem;
    }

    assert(curDim == -1);

    dimWidths[numDim - 1] = baseWidth;
    for(int i = numDim - 2;i >= 0;--i){
        dimWidths[i] = dimSizes[i + 1] * dimWidths[i + 1];
    }
    
    curDim = 0;
    curType = arrayTypePtr;
    while (curType->kind == ARRAY)
    {
        curType->info.array->numElem = dimSizes[curDim];
        curType->info.array->elemWidth = dimWidths[curDim];
        ++curDim;
        curType = curType->info.array->elem;
    }
    assert(curDim == numDim);

    free(dimSizes);//释放空间
    free(dimWidths);
}

void printError(int type,int line,char* msg){
    printf("Error Type %d at Line %d: %s\n",type,line,msg);
}

void printFuncDec(struct Func* funcPtr,int nrSpace){
    assert(funcPtr != NULL);
    printSpace(nrSpace);
    printf("function name:%s\n",funcPtr->name);
    printSpace(nrSpace);
    printf("function return type:\n");
    printType(funcPtr->retType,nrSpace+2);
    printSpace(nrSpace);
    printf("function parameters:\n");
    printFieldList(funcPtr->params,nrSpace+2);
}

unsigned hash(char *name,int sz)
{
    // unsigned val = 0, i;
    // for (; *name; ++name)
    // {
    //     val = (val << 2) + *name;
    //     if (i = val & ~sz)
    //         val = (val ^ (i >> 12)) & sz;
    // }
    // return val;


    if(hashType == 1){
        unsigned val = 0;
        for (; *name; ++name)
        {
            val += *name;
        }
        return val%sz;
    }

    else{
         unsigned int hash = 0;
    unsigned int x = 0;

    while (*name)
    {
        hash = (hash << 4) + (*name++);//hash左移4位，把当前字符ASCII存入hash低四位。 
        if ((x = hash & 0xF0000000L) != 0)
        {
            //如果最高的四位不为0，则说明字符多余7个，现在正在存第7个字符，如果不处理，再加下一个字符时，第一个字符会被移出，因此要有如下处理。
            //该处理，如果最高位为0，就会仅仅影响5-8位，否则会影响5-31位，因为C语言使用的算数移位
            //因为1-4位刚刚存储了新加入到字符，所以不能>>28
            hash ^= (x >> 24);
            //上面这行代码并不会对X有影响，本身X和hash的高4位相同，下面这行代码&~即对28-31(高4位)位清零。
            hash &= ~x;
        }
    }
    //返回一个符号位为0的数，即丢弃最高位，以免函数外产生影响。(我们可以考虑，如果只有字符，符号位不可能为负)
    return (hash & 0x7FFFFFFF)%sz;
     }
}

int checkTypeSame(struct Type *typePtr1,struct Type *typePtr2){
    //TODO 判断类型等价
    //先检查是否为NULL!
    //如果有一个是NULL，说明类型错误
    if((typePtr1 == NULL) || (typePtr2 == NULL)){
        return 0;
    }
    if(typePtr1 == typePtr2){
        //对于structure，如果指向同一个结构，那么就同类
        //也只有structure可能相同
        return 1;
    }
    if(typePtr1->kind != typePtr2->kind){
        return 0;//类型都不同
    }
    else{
        //现在同类
        if(typePtr1->kind == BASIC){
            return typePtr1->info.basicType == typePtr2->info.basicType;
        }
        else if(typePtr1->kind == ARRAY){
            //维数相同就行
            //递归
            return checkTypeSame(typePtr1->info.array->elem,typePtr2->info.array->elem);
        }
        else{
            //结构等价,就是每个域都等价
            struct FieldList * FL1 = typePtr1->info.structure->structureInfo;
            struct FieldList * FL2 = typePtr2->info.structure->structureInfo;
            while ((FL1 != NULL)&&(FL2 != NULL))
            {   
                //有一个不等价就不行
                if(checkTypeSame(FL1->type,FL2->type) == 0){
                    return 0;
                }
                FL1 = FL1->next;
                FL2 = FL2->next;
            }
            if((FL1 == NULL)&&(FL2 == NULL)){
                return 1;//前面全都等价
            }
            else{
                //域的数量不一致
                return 0;
            }
            
        }
    }
}

void checkNoDuplicateName(struct FieldList*FL){
    //用一个hashSet,自己写
    int sz = 10;
    struct NameNode** set = getNameHashSet(sz);//大小自己定
    while (FL != NULL)
    {
        if(nameSetContains(set,sz,FL->name)){
            printError(15,FL->line,"Structure field redifined.");
        }
        else{
            insertIntoSet(set,sz,FL->name);
        }
        FL = FL->next;
    }
    freeSet(set,sz);
}

int checkFieldListSame(struct FieldList*FL1,struct FieldList*FL2){
    //判断两个FL是否等价，注意FL可以为NULL，说明参数列表为空
    while ((FL1 != NULL)&&(FL2 != NULL))
    {
        if(checkTypeSame(FL1->type,FL2->type) == 0){
            return 0;
        }
        FL1 = FL1->next;
        FL2 = FL2->next;
    }
    if((FL1 == NULL)&&(FL2 == NULL)){
        return 1;//前面全都等价
    }
    else{
        //参数的数量不一致
        return 0;
    }
    
}

void insertFuncTable(struct Func* func){
    assert(func != NULL);
    assert(func->name != NULL);
    unsigned idx = hash(func->name,TABLE_SIZE);
    struct Func* head = funcTable[idx];
    //头部插入
    func->next = head;
    funcTable[idx] = func;
}

void insertStructureTable(struct Structure*structure){
    assert(structure != NULL);
    assert(structure->name != NULL);
    unsigned idx = hash(structure->name,TABLE_SIZE);
    struct Structure* head = structureTable[idx];
    //头部插入
    structure->next = head;
    structureTable[idx] = structure;
}

void insertVariableTable(struct Variable*variable){
    assert(variable != NULL);
    assert(variable->name != NULL);
    unsigned idx = hash(variable->name,TABLE_SIZE);
    struct Variable* head = variableTable[idx];
    //头部插入
    variable->next = head;
    variableTable[idx] = variable;
}

struct Func* searchFuncTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name,TABLE_SIZE);
    struct Func* head = funcTable[idx];
    numHashSearch++;
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
        numHashSearch++;
    }
    return head;
}

struct Structure* searchStructureTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name,TABLE_SIZE);
    struct Structure* head = structureTable[idx];
    numHashSearch++;
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        numHashSearch++;
        head = head->next;
    }
    return head;
}

struct Variable* searchVariableTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name,TABLE_SIZE);
    struct Variable* head = variableTable[idx];
    numHashSearch++;
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        numHashSearch++;
        head = head->next;
    }
    return head;
}

void printType(struct Type*typePtr,int nrSpace){
    if(typePtr != NULL){
        switch (typePtr->kind)
        {
            case BASIC:{
                if(typePtr->info.basicType == TYPE_INT){
                     printSpace(nrSpace);
                     printf("Kind:BASIC(int)\n");
                }
                else{
                    printSpace(nrSpace);
                    printf("Kind:BASIC(float)\n");
                }
            }break;
            case ARRAY:{
                printSpace(nrSpace);
                printf("Kind:ARRAY, numElem = %d, elemWidth = %d\n", typePtr->info.array->numElem, typePtr->info.array->elemWidth);
                printType(typePtr->info.array->elem,nrSpace+2);
            }break;
            case STRUCTURE:{
                printSpace(nrSpace);
                printf("Kind:STRUCTURE:%s\n",typePtr->info.structure->name);
                struct FieldList* ptr = typePtr->info.structure->structureInfo;
                while (ptr != NULL)
                {
                    printSpace(nrSpace);
                    printf("name=%s\n",ptr->name);
                    printType(ptr->type,nrSpace+2);
                    ptr = ptr->next;
                }    
            }break;
            default:{
                assert(0);
            }
        }
    }
    else{
        printSpace(nrSpace);             
        printf("Type = NULL\n");
    }
}

struct NameNode** getNameHashSet(int sz){
    assert(sz > 0);
    struct NameNode** ptr = (struct NameNode**)malloc(sizeof(struct NameNode*)*sz);
    memset(ptr,0,sizeof(struct NameNode*)*sz);
    return ptr;
}
int nameSetContains(struct NameNode** set,int sz,char* name){
    assert(name != NULL);
    int idx = hash(name,sz);
    struct NameNode* ptr = set[idx];
    while ((ptr != NULL) && (strcmp(ptr->name,name)!=0))
    {
        ptr = ptr -> next;
    }
    return (ptr != NULL);
}
void insertIntoSet(struct NameNode** set,int sz,char*name){
    assert(name != NULL);
    int idx = hash(name,sz);
    struct NameNode* ptr = (struct NameNode*)malloc(sizeof(struct NameNode));
    //头部插入
    ptr->name = name;
    ptr->next = set[idx];
    set[idx] = ptr;
}
void freeSet(struct NameNode** set,int sz){
    for(int i = 0;i < sz;++i){
        struct NameNode* ptr = set[i];
        struct NameNode* pre = NULL;
        while (ptr != NULL)
        {
            pre = ptr;
            ptr = ptr->next;
            free(pre);
        }
    }
}

void printFieldList(struct FieldList* ptr,int nrSpace){
    while (ptr != NULL)
    {
        printSpace(nrSpace);
        printf("name:%s\n",ptr->name);
        printType(ptr->type,nrSpace);
        ptr = ptr->next;
    }
    
}

void handleProgram(struct TreeNode* r){
    assert(r->type == Node_Program);
    if((r->numChildren == 1) && (r->children[0]->type == Node_ExtDefList)){
        handleExtDefList(r->children[0]);
    }
    else{
        assert(0);
    }
}


void handleExtDefList(struct TreeNode* r){
    assert(r->type == Node_ExtDefList);
    if(r->numChildren == 2){//ExtDefLust -> ExtDef ExtDefList
        handleExtDef(r->children[0]);
        handleExtDefList(r->children[1]);
    }
    else if(r->numChildren != 0){
        assert(0);
    }
}  

void handleExtDef(struct TreeNode* r){
    assert(r->type == Node_ExtDef);
    struct Type* typePtr = handleSpecifier(r->children[0]);
    if((r->numChildren == 3) && (r->children[2]->type == Node_SEMI)){
        //ExtDef -> Specifier ExtDecList SEMI
        struct FieldList * FL = handleExtDecList(r->children[1],typePtr);//传入类型，在内部创建变量
        // printFieldList(FL,0);
        //LAB3 没有全局变量
        assert(0);
    }
    else if((r->numChildren == 3) && (r->children[2]->type == Node_Compst)){
        //ExtDef -> Specifier FunDec Compst
        struct Func* funcPtr = handleFuncDec(r->children[1],typePtr);

        //增加ICNode
        struct InterCode* ICFuncPtr = newICNode(1);
        ICFuncPtr->kind = IC_FUNC_DEF;
        ICFuncPtr->operands[0]->kind = OPERAND_FUNC;
        ICFuncPtr->operands[0]->info.funcName = funcPtr->name;
        appendInterCodeToList(ICFuncPtr);

        struct FieldList * paramListPtr = funcPtr->params;
        while (paramListPtr != NULL)
        {
            struct InterCode* ICPramPtr = newICNode(1);
            ICPramPtr->kind = IC_PARAM;
            //形式参数也是变量，有全局作用域
            ICPramPtr->operands[0]->kind = OPEARND_VAR;
            ICPramPtr->operands[0]->info.varName = paramListPtr->name;
            appendInterCodeToList(ICPramPtr);
            paramListPtr = paramListPtr->next;
        }
        

        //函数头加入函数表
        if(searchFuncTable(funcPtr->name) == NULL){
            insertFuncTable(funcPtr);
        }
        else{
            //函数重定义
            printError(4,r->children[0]->line,"Function redefined.");
        }     

        //打印函数体 
        // printFuncDec(funcPtr,0);

        handleCompst(r->children[2],typePtr);//传入类型指针，遇到return判断返回值是否相容
    }
    else{
        //ExtDef -> Specifier Semi
        // printType(typePtr,0);
    }
}

struct Type* handleSpecifier(struct TreeNode* r){
    assert(r->type == Node_Sepcifier);
    struct Type* typePtr = NULL;
    if(r->children[0]->type == Node_TYPE){
        //Specifier -> TYPE 基本类型
        typePtr = (struct Type*)malloc(sizeof(struct Type));
        typePtr->kind = BASIC;
        typePtr->info.basicType = r->children[0]->val.typeVal;
    }
    else{
        //Specifier -> StructSpecifier 结构类型
        typePtr = handleStructSpecifier(r->children[0]);
    }
    return typePtr;
}

struct FieldList* handleExtDecList(struct TreeNode* r,struct Type* typePtr){
    assert(r->type == Node_ExtDecList);
    if(r->numChildren == 1){
        //ExtDecList -> VarDec
        struct FieldList* FL = handleVarDec2(r->children[0],typePtr);

        //反转数组
        if(FL->type->kind == ARRAY)
            reverseArray(FL->type);

        //插入变量表
        if((searchVariableTable(FL->name) != NULL)||(searchStructureTable(FL->name) != NULL)){
            //错误3，变量名重复定义
            printError(3,r->children[0]->line,"Variable name used before.");
        }
        else{
            //可以定义这个变量
            struct Variable* varPtr = getVarPtr(FL,r->children[0]->line);
            insertVariableTable(varPtr);
        }
        return FL;
    }
    else{
        //ExtDecList -> VarDec comma ExtDecList
        struct FieldList* FL1 = handleVarDec2(r->children[0],typePtr);//只有一个节点

        //反转数组
        if(FL1->type->kind == ARRAY)
            reverseArray(FL1->type);

        //即使变量重定义了，还是存在于结果FL中
        struct FieldList* FL2 = handleExtDecList(r->children[2],typePtr);//>=0个节点
        FL1->next = FL2;
        return FL1;
    }
}

struct FieldList* handleVarDec2(struct TreeNode* r,struct Type * typePtr){
    assert(r->type == Node_VarDec);
    struct FieldList* FL = (struct FieldList*)malloc(sizeof(struct FieldList));
    if(r->numChildren == 1){
        //VarDec -> ID
        FL->name = r->children[0]->idName;
        FL->next = NULL;
        FL->type = typePtr;
    }
    else{
        //VarDec -> VarDec LB INT RB
        //借助另一种VarDEc完成
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&(FL->name));//namePtr会被修改
        struct Type* arrayPtr = (struct Type*)malloc(sizeof(struct Type));
        arrayPtr->kind = ARRAY;
        arrayPtr->info.array = (struct Array*)malloc(sizeof(struct Array));
        arrayPtr->info.array->elem = typePtr2;
        arrayPtr->info.array->numElem = r->children[2]->val.intVal;
        FL->next = NULL;
        FL->type = arrayPtr;
        //Attention!这里数组维度是逆序，int a[2][3]的3在Array链表头部
    }
    return FL;
}

struct Type* handleStructSpecifier(struct TreeNode* r){
    assert(r->type == Node_StructSpecifier);
    struct Type* typePtr = NULL;
    if(r->numChildren == 2){
        //StructSpecifier -> Struct tag
        char* name = handleTag(r->children[1]);
        struct Structure * strucPtr = searchStructureTable(name);
        if(strucPtr == NULL){
            //TODO报错,接着返回什么呢???NULL???我返回NULL了
            printError(17,r->children[1]->line,"Undefined structure");
        }
        else{
            //找到
            typePtr = (struct Type*)malloc(sizeof(struct Type));
            typePtr->kind = STRUCTURE;
            typePtr->info.structure = strucPtr;
        }
        return typePtr;
    }
    else{
        //StructSpecifier -> Struct OptTag Lc DefList RC
        char* name = handleOptTag(r->children[1]);
        //考虑结构体重名???
        if((name != NULL) && ((searchStructureTable(name)!=NULL)||(searchVariableTable(name)!=NULL))){
            //结构体和之前定义的结构体或者变量重名
            printError(16,r->children[1]->line,"Structure name used before");
            //返回什么类型呢?NULL 没有类型,如果后面跟着一个变量呢?他就没有类型吗?
            //所以还是要有类型的，不能直接返回NULL,这里只要报错就行了
            // return NULL;
        }
        //name有可能是NULL，即匿名结构，就不要加入结构体表
        struct FieldList* FL = handleDefList(r->children[3],1);//struct里面的域不能初始化
        typePtr = (struct Type*)malloc(sizeof(struct Type));
        typePtr->kind = STRUCTURE;
        typePtr->info.structure = (struct Structure*)malloc(sizeof(struct Structure));
        typePtr->info.structure->structureInfo = FL;
        typePtr->info.structure->name = name;//名字是NULL也无所谓
        typePtr->info.structure->next = NULL;
        if(name != NULL){
            insertStructureTable(typePtr->info.structure);
        }
        //判断结构体内部有没有重名的域,有就报错,即使报错，也不影响类型的正常返回
        checkNoDuplicateName(FL);
        return typePtr;
    }
}

char* handleTag(struct TreeNode* r){
    assert(r->type == Node_Tag);
    //Tag -> ID
    return r->children[0]->idName;
}

char* handleOptTag(struct TreeNode* r){
    assert(r->type == Node_OptTag);
    char* name = NULL;
    if(r->numChildren == 1){
        name = r->children[0]->idName;
    }
    return name;
}

struct FieldList* handleDefList(struct TreeNode* r,int inStruc){
    assert(r->type == Node_DefList);
    if(r->numChildren == 0){
        //DefLst -> epsilon
        return NULL;
    }
    else{
        //DefLst -> Def DefList
        struct FieldList* FL1 = handleDef(r->children[0],inStruc);//数量>=1
        struct FieldList* FL2 = handleDefList(r->children[1],inStruc);//可能为空
        //找到FL1最后一个
        struct FieldList* tmp = FL1;
        while (tmp->next != NULL)
        {
            tmp = tmp -> next;
        }
        tmp ->next = FL2;
        return FL1;
    }
}

struct FieldList* handleDef(struct TreeNode* r,int inStruc){
    assert(r->type == Node_Def);
    //Def -> Specifier DecList Semi
    struct Type * typePtr = handleSpecifier(r->children[0]);
    return handleDecList(r->children[1],typePtr,inStruc);
}

struct FieldList* handleDecList(struct TreeNode* r,struct Type * typePtr,int inStruc){
    assert(r->type == Node_DecList);
    if(r->numChildren == 1){
        //DecList -> Dec
        return handleDec(r->children[0],typePtr,inStruc);
    }
    else{
        //DecList -> Dec comma Declist
        struct FieldList* FL1 = handleDec(r->children[0],typePtr,inStruc);
        struct FieldList* FL2 = handleDecList(r->children[2],typePtr,inStruc);
        FL1->next = FL2;
        return FL1;
    }
}

struct FieldList* handleDec(struct TreeNode* r,struct Type * typePtr,int inStruc){
    assert(r->type == Node_Dec);
    struct FieldList* FL = (struct FieldList*)malloc(sizeof(struct FieldList));
    if(r->numChildren == 1){
        //Dec -> VarDec
        char* name = NULL;
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&name);

        //反转数组
        if(typePtr2->kind == ARRAY)
            reverseArray(typePtr2);

        //如果是数组，中间代码要分配空间,Lab3 不会出现结构体，结构体报错
        if(typePtr2->kind == STRUCTURE){
            printf("Cannot translate: Code contains Variables or parameter of structure type.\n");
        }
        else if(typePtr2->kind == ARRAY){
            //数组只能以这种不带初始化的方式创建，因为数组不能赋值
            int spaceNeeded = calculateWidth(typePtr2);
            struct InterCode * ICPtr = newICNode(1);
            ICPtr->kind = IC_DEC;
            ICPtr->allocSize = spaceNeeded;
            ICPtr->operands[0]->kind = OPEARND_VAR;
            ICPtr->operands[0]->info.varName = name;
            appendInterCodeToList(ICPtr);
        }

        FL->name = name;//不空
        FL->type = typePtr2;//可能变成了数组
        FL->next = NULL;
        FL->line = r->children[0]->line; 
    }
    else{
        if(inStruc){
            //结构体内不能初始化
            printError(15,r->children[0]->line,"Cannot initialize field in structure.");
        }
        //就算不能初始化，如果出现语义错误，还是要报的
        //Dec -> VarDec Assign Exp
        char* name = NULL;
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&name);

        //反转数组
        if(typePtr2->kind == ARRAY)
            reverseArray(typePtr2);

        //如果是数组，中间代码要分配空间,Lab3 不会出现结构体，结构体报错
        if(typePtr2->kind == STRUCTURE){
            printf("Cannot translate: Code contains Variables or parameter of structure type.\n");
        }
        else if(typePtr2->kind == ARRAY){
            //这种情况不会发生，因为数组无法初始化
            // int spaceNeeded = calculateWidth(typePtr2);
            // struct InterCode * ICPtr = newICNode(1);
            // ICPtr->kind = IC_DEC;
            // ICPtr->allocSize = spaceNeeded;
            // ICPtr->operands[0]->kind = OPEARND_VAR;
            // ICPtr->operands[0]->info.varName = name;
            // appendInterCodeToList(ICPtr);
            assert(0);
        }

        //后面有赋值，只能是基本类型的初始化，要生成相应中间代码
        //先创建一个临时变量ti
        struct Operand* tmp = getNewTmpVar();
        //用Exp处理将tmp临时变量赋值，生成相应中间码
        struct Type* expTypePtr = handleExp(r->children[2],tmp, 1);//得到一个临时变量，如果是数组类型，要求取值
        assert(tmp->kind == OPEARND_TMP_VAR);
        struct InterCode * ICPtr = newICNode(1);//但是我需要2个，第二个自己初始化
        ICPtr->numOperands = 2;
        ICPtr->operands[1] = tmp;
        ICPtr->operands[0]->kind = OPEARND_VAR;
        ICPtr->operands[0]->info.varName = name;//name 不会是 NULL
        ICPtr->kind = IC_ASSIGN;
        appendInterCodeToList(ICPtr);

        FL->name = name;//不空
        FL->type = typePtr2;//可能变成了数组
        FL->next = NULL;
        FL->line = r->children[0]->line;
        if(checkTypeSame(expTypePtr,typePtr2) == 0){
            //赋值号两边表达式不匹配
            printError(5,r->children[1]->line,"Assignop mismatch.");
        }
    }
    //不在结构体中才插入变量表
    if(inStruc == 0){
        if((searchVariableTable(FL->name) == NULL)&&(searchStructureTable(FL->name) == NULL)){
            //不存在重定义
            struct Variable * varPtr = getVarPtr(FL,FL->line);
            varPtr->isParam = 0;//局部变量
            insertVariableTable(varPtr);
        }
        else{
            //报错，变量重定义
            printError(3,FL->line,"Variable redefined.");
        }
    }
    return FL;
}

struct Type* handleVarDec(struct TreeNode* r,struct Type * typePtr,char**namePtr){
    assert(r->type == Node_VarDec);
    if(r->numChildren == 1){
        //VarDec -> ID
        *namePtr = r->children[0]->idName;
        return typePtr;
    }
    else{
        //VarDec -> VarDec LB INT RB
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,namePtr);//namePtr会被修改
        struct Type* arrayPtr = (struct Type*)malloc(sizeof(struct Type));
        arrayPtr->kind = ARRAY;
        arrayPtr->info.array = (struct Array*)malloc(sizeof(struct Array));
        arrayPtr->info.array->elem = typePtr2;
        arrayPtr->info.array->numElem = r->children[2]->val.intVal;
        //Attention!这里数组维度是逆序，int a[2][3]的3在Array链表头部
        return arrayPtr;
    }
}

struct Variable* getVarPtr(struct FieldList*FL1,int line){
    struct Variable* varPtr = (struct Variable*)malloc(sizeof(struct Variable));
    varPtr->name = FL1->name;
    varPtr->firstAppearanceLine = line;
    varPtr->varType = FL1->type;
    varPtr->next = NULL;
}

struct Func* handleFuncDec(struct TreeNode* r,struct Type * typePtr){
    assert(r->type == Node_FuncDec);
    struct Func* funcPtr = (struct Func*)malloc(sizeof(struct Func));
    funcPtr->name = r->children[0]->idName;//ID
    funcPtr->next = NULL;
    funcPtr->retType = typePtr;
    if(r->numChildren == 3){
        //Func -> Id LP RP
        funcPtr->params = NULL;//没有参数
    }
    else{
        //Func -> Id LP VarList RP
        funcPtr->params = handleVarList(r->children[2]);
    }
    return funcPtr;
}

struct FieldList* handleVarList(struct TreeNode* r){
    assert(r->type == Node_VarList);
    if(r->numChildren == 1){
        //VarList -> ParamDec
        return handleParamDec(r->children[0]);
    }
    else{
        //VarList -> ParamDec comma ParamDec
        struct FieldList* FL1 = handleParamDec(r->children[0]);
        struct FieldList* FL2 = handleVarList(r->children[2]);
        FL1->next = FL2;
        return FL1;
    }
}

struct FieldList* handleParamDec(struct TreeNode* r){
    assert(r->type == Node_ParamDec);
    //ParamDec -> Specifier varDec
    struct Type* typePtr = handleSpecifier(r->children[0]);
    struct FieldList *FL = (struct FieldList *)malloc(sizeof(struct FieldList));
    FL->type = handleVarDec(r->children[1],typePtr,&(FL->name));

    //反转数组
    if(FL->type->kind == ARRAY)
        reverseArray(FL->type);

    FL->line = r->children[1]->line;
    FL->next = NULL;
    //检查重名参数，注意全局作用域
    if((searchStructureTable(FL->name) != NULL)||(searchVariableTable(FL->name) != NULL)){
        //变量(形参)和变量、结构体重名
        printError(3,FL->line,"Variable redefined.");
    }
    else{
        struct Variable* varPtr = getVarPtr(FL,FL->line);
        varPtr->isParam = 1;//是形参
        insertVariableTable(varPtr);
    }
    return FL;
}

void handleCompst(struct TreeNode* r,struct Type * typePtr){
    assert(r->type == Node_Compst);
    //Compst -> LC DefList StmtList RC
    struct FieldList * FL = handleDefList(r->children[1],0);//可以带初始化
    handleStmtList(r->children[2],typePtr);//判断return语句是否正确
}

void handleStmtList(struct TreeNode* r,struct Type * typePtr){
    assert(r->type == Node_StmtList);
    if(r->numChildren > 0){
        //StmtList -> Stmt StmtList
        handleStmt(r->children[0],typePtr);
        handleStmtList(r->children[1],typePtr);
    }
    //StmtList -> epsilon,don't care
}

void handleStmt(struct TreeNode* r,struct Type * typePtr){
    assert(r->type == Node_Stmt);
    if(r->numChildren == 1){
        //Stmt -> Compst
        handleCompst(r->children[0],typePtr);
    }
    else if(r->numChildren == 2){
        //Stmt -> Exp Semi
        handleExp(r->children[0], NULL,  0);//这个表达式的类型也不重要了
    }
    else if(r->numChildren == 3){
        //Stmt -> Return Exp Semi
        //判断返回值类型

        struct Operand * opRet = getNewTmpVar();
        //将函数的返回值存到临时变量中，如果是数组类型要求值
        struct Type * expTypePtr = handleExp(r->children[1], opRet, 1);
        //如果Exp返回类型为NULL,说明Exp里面已经报错，不用再报
        if((expTypePtr != NULL)&&checkTypeSame(expTypePtr,typePtr) == 0){
            printError(8,r->children[0]->line,"Return type mismatch.");
        }
        assert(opRet->kind == OPEARND_TMP_VAR);
        struct InterCode * ICPtr = newICNode(-1);
        ICPtr->numOperands = 1;
        ICPtr->operands[0] = opRet;
        ICPtr->kind = IC_RETURN;
        appendInterCodeToList(ICPtr);
    }
    else if(r->numChildren == 5){
        if(r->children[0]->type == Node_IF){
            //Stmt -> If LP Exp RP Stmt
            struct Type * expTypePtr = handleExp(r->children[2], NULL,  0);
            //TODO 判断exp的类型是不是INT,但是老师说不考虑，不管
            handleStmt(r->children[4],typePtr);//返回值留给Stmt判断
        }
        else{
            //和IF 一样的......
            //Stmt -> while LP Exp RP Stmt
            struct Type * expTypePtr = handleExp(r->children[2], NULL,  0);
            //TODO 判断exp的类型是不是INT,但是老师说不考虑，不管
            handleStmt(r->children[4],typePtr);//返回值留给Stmt判断
        }
    }
    else{
        //Stmt -> If LP Exp RP Stmt else Stmt
        struct Type * expTypePtr = handleExp(r->children[2], NULL,  0);
        //TODO 判断exp的类型是不是INT,但是老师说不考虑，不管
        handleStmt(r->children[4],typePtr);//返回值留给Stmt判断
        handleStmt(r->children[6],typePtr);//返回值留给Stmt判断
    }
}

struct Type * handleExp(struct TreeNode* r, struct Operand* place, int needGetValue){
    assert(r->type == Node_Exp);
    if(r->numChildren == 1){
        if(r->children[0]->type == Node_ID){
            //Exp -> ID
            //查找变量表
            struct Variable * varPtr = searchVariableTable(r->children[0]->idName);
            if(varPtr == NULL){
                //没有这个变量
                printError(1,r->children[0]->line,"Variable Undefined.");
                return NULL;//这里的NULL实在没办法忽视了，真的没有类型
            }
            else{
                if(place != NULL){
                    //ID可能是基本类型或者基本类型数组(不会出现结构)
                    //place的名字已经确定了
                    if(varPtr->varType->kind == BASIC){
                        //生成一条赋值的中间代码
                        struct InterCode * ICPtr = newICNode(-1);
                        ICPtr->kind = IC_ASSIGN;//基本的赋值
                        struct Operand* opPtr1 = newOperand();
                        opPtr1->kind = OPEARND_VAR;
                        opPtr1->info.varName = varPtr->name;
                        place->kind = OPEARND_TMP_VAR;
                        ICPtr->operands[0] = place;
                        ICPtr->operands[1] = opPtr1;
                        ICPtr->numOperands = 2;
                        appendInterCodeToList(ICPtr);
                    }
                    else if(varPtr->varType->kind == ARRAY){
                        //数组的话，ID应该已经在中间代码DEC过了。
                        //记录偏移量和base
                        struct InterCode * ICPtr = newICNode(-1);
                        ICPtr->kind = IC_ASSIGN;//x := 0，最后一步才是加上&base和加上偏移量
                        struct Operand* opPtr1 = newOperand();
                        opPtr1->kind = OPEARND_CONSTANT;//常数
                        opPtr1->info.constantVal = 0;
                        place->kind = OPEARND_ADDR;//place对应中间代码变量ti已经确定了
                        place->baseName = varPtr->name;
                        ICPtr->operands[0] = place;
                        ICPtr->operands[1] = opPtr1;
                        ICPtr->numOperands = 2;
                        //目前place->info.address.vartmpno只是偏移量
                        appendInterCodeToList(ICPtr);
                    }
                    else
                    {
                        assert(0);
                    }
                    
                }
                return varPtr->varType;
            }
        }
        else if(r->children[0]->type == Node_INT){
            //Exp -> INT
            //创建一个Type*
            struct Type * typePtr = (struct Type *)malloc(sizeof(struct Type));
            typePtr->kind = BASIC;
            typePtr->info.basicType = TYPE_INT;

            if(place != NULL){
                //place的名字已经确定了
                //生成一条赋值的中间代码
                struct InterCode * ICPtr = newICNode(-1);
                ICPtr->kind = IC_ASSIGN;//基本的赋值
                struct Operand* opPtr1 = newOperand();
                opPtr1->kind = OPEARND_CONSTANT;
                opPtr1->info.constantVal = r->children[0]->val.intVal;
                place->kind = OPEARND_TMP_VAR;
                ICPtr->operands[0] = place;
                ICPtr->operands[1] = opPtr1;
                ICPtr->numOperands = 2;
                appendInterCodeToList(ICPtr);
            }

            return typePtr;
        }
        else{
            //Exp -> FLAOT
            //创建一个Type*
            assert(0);//lab3没有float
            struct Type * typePtr = (struct Type *)malloc(sizeof(struct Type));
            typePtr->kind = BASIC;
            typePtr->info.basicType = TYPE_FLOAT;
            return typePtr;
        }
    }
    else if(r->numChildren == 2){
        if(r->children[0]->type == Node_NOT){
            //Exp -> Not Exp
            //判断Exp 是不是NULL或者basic类型
            struct Type * typePtr = handleExp(r->children[1], NULL,  0);//有可能得到NULL
            if((typePtr == NULL) || (typePtr->kind != BASIC) ){
                //操作数类型不匹配,Not只能跟基本类型INT或者FLoat(我用C语言试了)
                printError(7,r->children[0]->line,"Variable(s) type mismatch.");
                return NULL;//这里返回????没得返回
            }
            else{
                return typePtr;//类型不变
            }
        }
        else{
            //Exp -> Neg Exp
            //判断Exp 是不是NULL或者basic类型
            struct Operand * t1 = getNewTmpVar();
            t1->kind = OPEARND_CONSTANT;
            t1->info.constantVal = 0;
            struct Operand * t2 = getNewTmpVar();
            struct Type * typePtr = handleExp(r->children[1], t2,  1);
            if((typePtr == NULL) || (typePtr->kind != BASIC) ){
                //操作数类型不匹配
                printError(7,r->children[0]->line,"Variable(s) type mismatch.");
                return NULL;//????
            }
            else{
                assert(t1->kind == OPEARND_CONSTANT && t2->kind == OPEARND_TMP_VAR);
                struct InterCode *INPtr = newICNode(-1);
                INPtr->numOperands = 3;
                INPtr->operands[0] = place;
                INPtr->operands[1] = t1;
                INPtr->operands[2] = t2;
                INPtr->kind = IC_SUB;
                appendInterCodeToList(INPtr);
                return typePtr;//类型不变
            }
        }
    }
    else if(r->numChildren == 3){
        if(r->children[1]->type == Node_ASSIGNOP){
            //赋值号左边只能是左值，仅从语法上检查,Exp只能是ID | Exp LB EXP RB | Exp Dot ID
            if(((r->children[0]->numChildren == 1) && (r->children[0]->children[0]->type == Node_ID))
                ||((r->children[0]->numChildren == 4) && (r->children[0]->children[1]->type == Node_LB))
                ||((r->children[0]->numChildren == 3) && (r->children[0]->children[1]->type == Node_DOT))){
                    //什么都不做(^-^)
                }
            else{
                printError(6,r->children[1]->line,"The left-hand side of an assignment must be a variable.");
                //报错之后，还要分析吗?????
                //暂时继续分析吧TODO may FIX it
            }
            //Exp -> Exp Assign Exp
            struct Operand * left = getNewTmpVar();
            struct Operand * right = getNewTmpVar();

            struct Type * typePtr1 = handleExp(r->children[0], left,  0);//如果是数组需要不需要取值
            struct Type * typePtr2 = handleExp(r->children[2], right,  1);
            //这两个都有可能返回NULL,如果有一个NULL，说明其中一个Exp的类型不存在(出错)
            if((typePtr1 == NULL)||(typePtr2 == NULL)){
                //如果有一个是NULL,说明之前已经错了，这里就不报错了
                return NULL;
            }
            if(checkTypeSame(typePtr1,typePtr2) == 1){//类型相同，非空
                //生成赋值语句中间代码
                struct InterCode * ICPtr = newICNode(-1);
                ICPtr->numOperands = 2;
                ICPtr->operands[0] = left;
                ICPtr->operands[1] = right;
                if(left->kind == OPEARND_ADDR){
                    //*x := y
                    assert(left->kind == OPEARND_ADDR);
                    ICPtr->kind = IC_WRITE_TO_ADDR;
                }
                else{//左侧是一个普通变量,直接赋值给变量,这会使得之前那个left产生了一个废话ti =  left
                    ICPtr->operands[0] = newOperand();
                    ICPtr->operands[0]->kind = OPEARND_VAR;
                    ICPtr->operands[0]->info.varName = r->children[0]->children[0]->idName;
                    ICPtr->kind = IC_ASSIGN;
                }
                appendInterCodeToList(ICPtr);
                if(place != NULL){
                    //给place赋值
                    ICPtr = newICNode(-1);
                    ICPtr->numOperands = 2;
                    ICPtr->operands[0] = place;
                    place->kind = OPEARND_TMP_VAR;
                    ICPtr->operands[1] = right;
                    ICPtr->kind = IC_ASSIGN;
                    appendInterCodeToList(ICPtr);
                }
                return typePtr1;
            }
            else{
                //报错,赋值号类型不匹配
                printError(5,r->children[1]->line,"Assignop mismatch.");
                return NULL;//不能赋值就返回NULL
            }
            
        }
        else if(r->children[1]->type == Node_AND){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0], NULL,  0);
            struct Type * typePtr2 = handleExp(r->children[2], NULL,  0);
            if((typePtr1 == NULL)||(typePtr2 == NULL)){
                //如果有一个是NULL,说明之前已经错了，这里就不报错了
                return NULL;
            }
            //BASIC才能AND
            if((checkTypeSame(typePtr1,typePtr2) == 1) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                //操作数不匹配
                printError(7,r->children[1]->line,"Oprand(s) or operator mismatch.");
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_OR){
            //Exp -> Exp OR Exp
            struct Type * typePtr1 = handleExp(r->children[0], NULL,  0);
            struct Type * typePtr2 = handleExp(r->children[2], NULL,  0);
            if((typePtr1 == NULL)||(typePtr2 == NULL)){
                //如果有一个是NULL,说明之前已经错了，这里就不报错了
                return NULL;
            }
            //整数才能OR
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                printError(7,r->children[1]->line,"Oprand(s) or operator mismatch.");
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_RELOP){
            //Exp -> Exp ReLop Exp
            struct Type * typePtr1 = handleExp(r->children[0], NULL,  0);
            struct Type * typePtr2 = handleExp(r->children[2], NULL,  0);
            if((typePtr1 == NULL)||(typePtr2 == NULL)){
                //如果有一个是NULL,说明之前已经错了，这里就不报错了
                return NULL;
            }
            //基本类型才能RELOP
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                // return typePtr1;
                //RELOP类型是INT
                struct Type * ptr = (struct Type *)malloc(sizeof(struct Type));
                ptr->kind = BASIC;
                ptr->info.basicType = TYPE_INT;
                return ptr;//是INT类型
            }
            else{
                printError(7,r->children[1]->line,"Oprand(s) or operator mismatch.");
                return NULL;//不能赋值就返回NULL
            }
        }
        else if((r->children[1]->type == Node_PLUS)||(r->children[1]->type == Node_MINUS)
        ||(r->children[1]->type == Node_STAR)||(r->children[1]->type == Node_DIV)){
            //Exp -> Exp Assign Exp
            struct Operand * t1 = getNewTmpVar();
            struct Operand * t2 = getNewTmpVar();
            struct Type * typePtr1 = handleExp(r->children[0], t1,  1);
            struct Type * typePtr2 = handleExp(r->children[2], t2,  1);
            if((typePtr1 == NULL)||(typePtr2 == NULL)){
                //如果有一个是NULL,说明之前已经错了，这里就不报错了
                return NULL;
            }
            //基本类型才能PLUS
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                assert(t1->kind == OPEARND_TMP_VAR &&t2->kind == OPEARND_TMP_VAR);
                struct InterCode *INPtr = newICNode(-1);
                INPtr->numOperands = 3;
                INPtr->operands[0] = place;
                INPtr->operands[1] = t1;
                INPtr->operands[2] = t2;
                switch (r->children[1]->type)
                {
                case Node_PLUS:{
                    INPtr->kind = IC_PLUS;
                }break;
                case Node_MINUS:{
                    INPtr->kind = IC_SUB;
                }break;
                case Node_STAR:{
                    INPtr->kind = IC_MUL;
                }break;
                case Node_DIV:{
                    INPtr->kind = IC_DIV;
                }break;
                default:{
                    assert(0);
                }break;
                }
                appendInterCodeToList(INPtr);
                return typePtr1;
            }
            else{
                printError(7,r->children[1]->line,"Oprand(s) or operator mismatch.");
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[0]->type == Node_LP){
            //Exp -> LP Exp RP
           return handleExp(r->children[1], NULL,  0);
        }
        else if(r->children[1]->type == Node_LP){
            //Exp -> ID LP RP
            if(strcmp(r->children[0]->idName, "read") == 0){
                //读取一个值先放到临时变量place中，再做其他处理
                assert(place->kind == OPEARND_TMP_VAR);
                struct InterCode * ICPtr = newICNode(-1);
                ICPtr->numOperands = 1;
                ICPtr->operands[0] = place;
                ICPtr->kind = IC_READ;
                appendInterCodeToList(ICPtr);
                //返回的类型是INT
                return intTypePtr;
            }
            //函数调用
            struct Func * funcPtr = searchFuncTable(r->children[0]->idName);
            if(funcPtr == NULL){
                if(searchVariableTable(r->children[0]->idName) != NULL){
                    //对普通变量使用()
                    printError(11,r->children[0]->line,"Try use () to normal variable.");
                }
                else//调用未定义的函数
                    printError(2,r->children[0]->line,"Function Undefined.");
                return NULL;
            }
            else{
                //函数定义了，判断参数是否符合
                if(checkFieldListSame(funcPtr->params,NULL)){
                    return funcPtr->retType;//返回的是函数返回类型
                }
                else{
                    printError(9,r->children[0]->line,"Function call parameters mismatch.");
                    return NULL;
                }
            }
        }
        else{
            //Exp -> Exp DOT ID
            assert(r->children[1]->type == Node_DOT);//前面太多了，assert一下
            struct Type * expTypePtr = handleExp(r->children[0], NULL,  0);
            //判断ptr是否非NULL且为Structure并且有域ID
            if(expTypePtr == NULL){
                //如果是NULL,说明递归exp已经错了，这里就不报错了
                return NULL;
            }
            if((expTypePtr != NULL)&&(expTypePtr->kind == STRUCTURE)){
                struct FieldList * FL = expTypePtr->info.structure->structureInfo;//结构体的域
                while ((FL != NULL) && (strcmp(FL->name,r->children[2]->idName) != 0))
                {
                    FL = FL -> next;
                }
                if(FL == NULL){
                    //没有这个域14
                    printError(14,r->children[1]->line,"Access undefined field in structure.");
                    return NULL;
                }
                else{
                    return FL->type;
                }
            }
            else{
                //Exp的类型不是结构体
                //对非结构体使用.操作
                printError(13,r->children[1]->line,"Use '.' to non-Strcuture.");
                return NULL;
            }
        }
    }
    else{
        //num == 4 
        assert(r->numChildren == 4);
        if(r->children[0]->type == Node_ID){
            //Exp -> ID LP Args RP
            struct Func * funcPtr = searchFuncTable(r->children[0]->idName);
            if(funcPtr == NULL){
                if(searchVariableTable(r->children[0]->idName) != NULL){
                    //对普通变量使用()
                    printError(11,r->children[0]->line,"Try use () to normal variable.");
                }
                else//调用未定义的函数
                    printError(2,r->children[0]->line,"Function Undefined.");
                return NULL;
            }
            else{
                //函数定义了，判断参数是否符合
                //先得到Args:FL,只有type重要,name不需要
                struct FieldList* FL = handleArgs(r->children[2]);
                if(checkFieldListSame(funcPtr->params,FL)){
                    if(strcmp(r->children[0]->idName, "write") == 0){
                        //把第一个参数exp的值放到一个局部变量
                        struct Operand * tmp = getNewTmpVar();
                        handleExp(r->children[2]->children[0], tmp, 1);
                        assert(tmp->kind == OPEARND_TMP_VAR);
                        struct InterCode * ICPtr = newICNode(-1);
                        ICPtr->numOperands = 1;
                        ICPtr->operands[0] = tmp;
                        ICPtr->kind = IC_WRITE;
                        appendInterCodeToList(ICPtr);
                    }
                    return funcPtr->retType;//返回的是函数返回类型
                }
                else{
                    printError(9,r->children[0]->line,"Function call parameters mismatch.");
                    return NULL;
                }
            }
        }
        else{
            //Exp -> Exp LB Exp RB
            assert(r->children[0]->type == Node_Exp);

            struct Operand *tmpIdx = getNewTmpVar();
            struct Operand *preOffset = getNewTmpVar();
            struct Type * typePtr1 = handleExp((r->children[0]), preOffset,  0);//将preOffset设置好
            struct Type * typePtr2 = handleExp((r->children[2]), tmpIdx,  1);//得到下标值
            //1应该是array,2是INT
            if((typePtr1 != NULL)&&(typePtr1->kind==ARRAY)&&(typePtr2 != NULL)
                &&(typePtr2->kind==BASIC)&&(typePtr2->info.basicType == TYPE_INT)){
                    assert(preOffset->kind == OPEARND_ADDR);

                    //要新建一个临时变量计算新的偏移量
                    //先计算要加上的偏移量，用idx*width得到
                    struct Operand * addOffset = getNewTmpVar();
                    struct Operand * width = newOperand();
                    width->kind = OPEARND_CONSTANT;
                    width->info.constantVal = typePtr1->info.array->elemWidth;
                    struct InterCode * ICPtr = newICNode(-1);
                    ICPtr->numOperands = 3;
                    ICPtr->operands[0] = addOffset;
                    ICPtr->operands[1] = tmpIdx;
                    ICPtr->operands[2] = width;
                    ICPtr->kind = IC_MUL;
                    appendInterCodeToList(ICPtr);
                    //现在addoffest对应的临时变量已经记录的新加偏移量了
                    //得到加和总偏移量
                    struct Operand * sumOffset = getNewTmpVar();//产生一个新的临时变量
                    ICPtr = newICNode(-1);
                    ICPtr->numOperands = 3;
                    ICPtr->operands[0] = sumOffset;
                    ICPtr->operands[1] = preOffset;//ADDR类型
                    ICPtr->operands[2] = addOffset;
                    ICPtr->kind = IC_PLUS;
                    appendInterCodeToList(ICPtr);

                    if(typePtr1->info.array->elem->kind != ARRAY){
                        //当前是最后一个，还要进行&得到base以及+base到place
                        //得到base到临时变量，根据是形参还是局部变觉得要不要&
                        struct Operand * base = getNewTmpVar();

                        if(searchVariableTable(preOffset->baseName)->isParam == 0){
                            //局部数组要取地址
                            ICPtr = newICNode(-1);
                            ICPtr->numOperands = 2;
                            ICPtr->operands[0] = base;
                            ICPtr->operands[1] = newOperand();
                            ICPtr->operands[1]->kind = OPEARND_VAR;
                            ICPtr->operands[1]->info.varName = preOffset->baseName;
                            // ICPtr->operands[1]->baseName = preOffset->baseName;
                            ICPtr->kind = IC_GET_ADDR;//base := & x
                            appendInterCodeToList(ICPtr);
                        }
                        else{
                            //参数数组，已经是地址了，用一个多余的赋值操作
                            ICPtr = newICNode(-1);
                            ICPtr->numOperands = 2;
                            ICPtr->operands[0] = base;
                            ICPtr->operands[1] = newOperand();
                            ICPtr->operands[1]->kind = OPEARND_VAR;
                            ICPtr->operands[1]->info.varName = preOffset->baseName;
                            ICPtr->kind = IC_ASSIGN;//base :=  x，形式参数本来就是地址
                            appendInterCodeToList(ICPtr);
                        }
                        //现在base对应的变量保存了基地址，加上偏移量
                        if(needGetValue == 0)
                        {
                            ICPtr = newICNode(-1);
                            ICPtr->numOperands = 3;
                            ICPtr->operands[0] = place;
                            place->baseName = preOffset->baseName;
                            place->kind = OPEARND_ADDR;
                            ICPtr->operands[1] = base;//tmpVar
                            ICPtr->operands[2] = sumOffset;//tmpVar
                            ICPtr->kind = IC_PLUS;
                            appendInterCodeToList(ICPtr);
                        }
                        else{
                            //取地址上的数值
                            ICPtr = newICNode(-1);
                            ICPtr->numOperands = 3;
                            struct Operand *absAddr = getNewTmpVar();
                            ICPtr->operands[0] = absAddr;
                            absAddr->baseName = preOffset->baseName;
                            absAddr->kind = OPEARND_ADDR;
                            ICPtr->operands[1] = base;//tmpVar
                            ICPtr->operands[2] = sumOffset;//tmpVar
                            ICPtr->kind = IC_PLUS;
                            appendInterCodeToList(ICPtr);

                            ICPtr = newICNode(-1);
                            ICPtr->numOperands = 2;
                            ICPtr->operands[0] = place;
                            place->kind = OPEARND_TMP_VAR;
                            ICPtr->operands[1] = absAddr;
                            ICPtr->kind = IC_GET_VALUE;
                            appendInterCodeToList(ICPtr);
                        }
                    }
                    else{
                        //得到base到临时变量
                        ICPtr = newICNode(-1);
                        ICPtr->numOperands = 2;
                        ICPtr->operands[0] = place;
                        place->baseName = preOffset->baseName;
                        place->kind = OPEARND_ADDR;
                        ICPtr->operands[1] = sumOffset;
                        ICPtr->kind = IC_ASSIGN;
                        appendInterCodeToList(ICPtr);
                    }

                    return typePtr1->info.array->elem;//返回上一层类型
                }
            else{
                if(!((typePtr1 != NULL)&&(typePtr1->kind==ARRAY))){
                    //非数组使用[]
                    printError(10,r->children[1]->line,"Use [] to no-Array.");
                    return NULL;
                }
                else{
                    //[]非整数,也只是内部出错，还是返回子类型,不影响后续分析
                    printError(12,r->children[1]->line,"no-Interger in [].");
                    return typePtr1->info.array->elem;
                }
                // return NULL;//没办法，只能NULL
            }
        }
    }
}

struct FieldList* handleArgs(struct TreeNode* r){
    assert(r->type == Node_Args);
    if(r->numChildren == 1){
        //Args -> Exp
        struct FieldList* FL = (struct FieldList*)malloc(sizeof(struct FieldList));
        FL->name = NULL;
        FL->type = handleExp(r->children[0], NULL,  0);
        FL->line = r->children[0]->line;
        FL->next = NULL;
        return FL;
    }
    else{
        //Args -> Exp Comma Args
        struct FieldList* FL1 = (struct FieldList*)malloc(sizeof(struct FieldList));
        FL1->type =  handleExp(r->children[0], NULL,  0);//单个
        FL1->name = NULL;
        FL1->line = r->children[1]->line;
        struct FieldList* FL2 = handleArgs(r->children[2]);
        FL1->next = FL2;
        return FL1;//连上
    }
}

void appendInterCodeToList(struct InterCode* ICNodePtr){
    assert(ICNodePtr != NULL);
    //ICNodePtr只是一个节点
    //for debug
    // ICNodePtr->next = ICNodePtr->prev = ICNodePtr;
    printICPtr(stderr, ICNodePtr);
    if(InterCodeList == NULL){
        //初始情况
        InterCodeList = ICNodePtr;
        InterCodeList->next = InterCodeList->prev = InterCodeList;
    }
    else{
        ICNodePtr->next = InterCodeList;
        ICNodePtr->prev = InterCodeList->prev;
        InterCodeList->prev->next = ICNodePtr;
        InterCodeList->prev = ICNodePtr;
    }
    
}

struct InterCode* newICNode(int numOperands){
    struct InterCode* ptr = (struct InterCode*)malloc(sizeof(struct InterCode));
    if(numOperands == -1){
        return ptr;//调用者自己完成
    }
    ptr->numOperands = numOperands;
    for(int i = 0;i < numOperands;++i){
        ptr->operands[i] = (struct Operand *)malloc(sizeof(struct Operand));
    }
    return ptr;
}

//获取新的跳转标号
struct Operand* getNewLabel(){
    struct Operand* labelOp = (struct Operand*)malloc(sizeof(struct Operand));
    labelOp->kind = OPERAND_LABEL;
    labelOp->info.laeblNo = nrLabel;
    nrLabel++;
    return labelOp;
}

//获取新的临时变量
struct Operand* getNewTmpVar(){
    struct Operand* tmpValOp = (struct Operand*)malloc(sizeof(struct Operand));
    tmpValOp->kind = OPEARND_TMP_VAR;//这个不一定
    tmpValOp->info.tmpVarNo = nrTmpVar;
    nrTmpVar++;
    return tmpValOp;
}

void printICPtr(FILE* fd, struct InterCode * curPtr){
    switch (curPtr->kind)
        {
        case IC_FUNC_DEF:{
            assert(curPtr->numOperands == 1);
            fprintf(fd,"FUNCTION %s :\n",curPtr->operands[0]->info.funcName);
        }break;
        case IC_PARAM:{
            assert(curPtr->numOperands == 1);
            fprintf(fd,"PARAM %s\n",curPtr->operands[0]->info.varName);
        }break;
        case IC_DEC:{
            assert(curPtr->numOperands == 1);
            fprintf(fd,"DEC %s %d\n",curPtr->operands[0]->info.varName,curPtr->allocSize);
        }break;
        case IC_ASSIGN:{
            assert(curPtr->numOperands == 2);
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := ");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, "\n");
        }break;
        case IC_GET_ADDR:{
            assert((curPtr->numOperands == 2) && (curPtr->operands[1]->kind == OPEARND_VAR));
             printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := &");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, "\n");
        }break;
        case IC_PLUS:{
            assert((curPtr->numOperands == 3));
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := ");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, " + ");
            printOperand(fd, curPtr->operands[2]);
            fprintf(fd, "\n");
        }break;
        case IC_SUB:{
            assert((curPtr->numOperands == 3));
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := ");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, " - ");
            printOperand(fd, curPtr->operands[2]);
            fprintf(fd, "\n");
        }break;
        case IC_MUL:{
            assert((curPtr->numOperands == 3));
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := ");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, " * ");
            printOperand(fd, curPtr->operands[2]);
            fprintf(fd, "\n");
        }break;
         case IC_DIV:{
            assert((curPtr->numOperands == 3));
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := ");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, " / ");
            printOperand(fd, curPtr->operands[2]);
            fprintf(fd, "\n");
        }break;
        case IC_GET_VALUE:{
            assert((curPtr->numOperands == 2));
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := *");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, "\n");
        }break;
        case IC_WRITE_TO_ADDR:{
            assert((curPtr->numOperands == 2) && (curPtr->operands[0]->kind == OPEARND_ADDR));
            fprintf(fd, "*");
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, " := ");
            printOperand(fd, curPtr->operands[1]);
            fprintf(fd, "\n");
        }break;
        case IC_RETURN:{
            assert((curPtr->numOperands == 1) && (curPtr->operands[0]->kind == OPEARND_TMP_VAR));
            fprintf(fd, "RETURN ");
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, "\n");
        }break;
        case IC_READ:{
            assert((curPtr->numOperands == 1) && (curPtr->operands[0]->kind == OPEARND_TMP_VAR));
            fprintf(fd, "READ ");
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, "\n");
        }break;
        case IC_WRITE:{
            assert((curPtr->numOperands == 1));
            fprintf(fd, "WRITE ");
            printOperand(fd, curPtr->operands[0]);
            fprintf(fd, "\n");
        }break;
        default:
            break;
        }
}

void printInterCodeList(FILE* fd){
    if(InterCodeList == NULL){
        return;
    }
    struct InterCode * curPtr = InterCodeList;
    do
    {
        printICPtr(fd, curPtr);
        curPtr = curPtr->next;
    } while (curPtr != InterCodeList);

}

struct Operand*  newOperand(){
    struct Operand* ptr = (struct Operand*)malloc(sizeof(struct Operand));
    return ptr;
}

void printOperand(FILE* fd, struct Operand* op){
    switch (op->kind)
    {
    case OPEARND_VAR:{
        fprintf(fd, "%s", op->info.varName);
    }break;
    case OPEARND_TMP_VAR:
    case OPEARND_ADDR:
    {
        fprintf(fd, "t%d", op->info.tmpVarNo);
    }break;
    case OPEARND_CONSTANT:{
        fprintf(fd, "#%d", op->info.constantVal);
    }break;
    default:
        assert(0);
    }
}

void initReadAndWrite(){
    intTypePtr = (struct Type *)malloc(sizeof(struct Type));
    intTypePtr->kind = BASIC;
    intTypePtr->info.basicType = TYPE_INT;   
    struct Func* readFunc = (struct Func*)malloc(sizeof(struct Func));
    readFunc->name = "read";
    readFunc->params = NULL;
    readFunc->retType = intTypePtr;

    struct Func* writeFunc = (struct Func*)malloc(sizeof(struct Func));
    writeFunc->name = "write";
    struct FieldList * FL = (struct FieldList *)malloc(sizeof(struct FieldList));
    FL->next = NULL;
    FL->type = intTypePtr;
    writeFunc->params = FL;
    writeFunc->retType = NULL;

    insertFuncTable(readFunc);
    insertFuncTable(writeFunc);
}