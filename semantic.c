#include "semantic.h"
#include "tree.h"
#include <assert.h>
#include <string.h>

struct Variable *variableTable[TABLE_SIZE];
struct Structure *structureTable[TABLE_SIZE];
struct Func *funcTable[TABLE_SIZE];

void printError(int type,int line,char* msg){
    fprintf(stderr,"Error type %d at line %d:%s\n",type,line,msg);
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
    unsigned val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~sz)
            val = (val ^ (i >> 12)) & sz;
    }
    return val;
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
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}

struct Structure* searchStructureTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name,TABLE_SIZE);
    struct Structure* head = structureTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}

struct Variable* searchVariableTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name,TABLE_SIZE);
    struct Variable* head = variableTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
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
                printf("Kind:ARRAY,size = %d\n",typePtr->info.array->size);
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
        printFieldList(FL,0);
    }
    else if((r->numChildren == 3) && (r->children[2]->type == Node_Compst)){
        //ExtDef -> Specifier FunDec Compst
        //TODO 函数部分
        struct Func* funcPtr = handleFuncDec(r->children[1],typePtr);
        //函数头加入函数表
        if(searchFuncTable(funcPtr->name) == NULL){
            insertFuncTable(funcPtr);
        }
        else{
            //函数重定义
            printError(4,r->children[0]->line,"Function redefined.");
        }     

        //打印函数体 
        printFuncDec(funcPtr,0);

        handleCompst(r->children[2],typePtr);//传入类型指针，遇到return判断返回值是否相容
    }
    else{
        //ExtDef -> Specifier Semi
        printType(typePtr,0);
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
        arrayPtr->info.array->size = r->children[2]->val.intVal;
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
            //返回什么类型呢?NULL 没有类型
            return NULL;
        }
        //name有可能是NULL，即匿名结构，就不要加入结构体表
        struct FieldList* FL = handleDefList(r->children[3],1);
        typePtr = (struct Type*)malloc(sizeof(struct Type));
        typePtr->kind = STRUCTURE;
        typePtr->info.structure = (struct Structure*)malloc(sizeof(struct Structure));
        typePtr->info.structure->structureInfo = FL;
        typePtr->info.structure->name = name;//名字是NULL也无所谓
        typePtr->info.structure->next = NULL;
        if(name != NULL){
            insertStructureTable(typePtr->info.structure);
        }
        //判断结构体内部有没有重名的域,有就报错
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

struct FieldList* handleDefList(struct TreeNode* r,int noExp){
    assert(r->type == Node_DefList);
    if(r->numChildren == 0){
        //DefLst -> epsilon
        return NULL;
    }
    else{
        //DefLst -> Def DefList
        struct FieldList* FL1 = handleDef(r->children[0],noExp);//数量>=1
        struct FieldList* FL2 = handleDefList(r->children[1],noExp);//可能为空
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

struct FieldList* handleDef(struct TreeNode* r,int noExp){
    assert(r->type == Node_Def);
    //Def -> Specifier DecList Semi
    struct Type * typePtr = handleSpecifier(r->children[0]);
    return handleDecList(r->children[1],typePtr,noExp);
}

struct FieldList* handleDecList(struct TreeNode* r,struct Type * typePtr,int noExp){
    assert(r->type == Node_DecList);
    if(r->numChildren == 1){
        //DecList -> Dec
        return handleDec(r->children[0],typePtr,noExp);
    }
    else{
        //DecList -> Dec comma Declist
        struct FieldList* FL1 = handleDec(r->children[0],typePtr,noExp);
        struct FieldList* FL2 = handleDecList(r->children[2],typePtr,noExp);
        FL1->next = FL2;
        return FL1;
    }
}

struct FieldList* handleDec(struct TreeNode* r,struct Type * typePtr,int noExp){
    assert(r->type == Node_Dec);
    struct FieldList* FL = (struct FieldList*)malloc(sizeof(struct FieldList));
    if(r->numChildren == 1){
        //Dec -> VarDec
        char* name = NULL;
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&name);
        FL->name = name;//不空
        FL->type = typePtr2;//可能变成了数组
        FL->next = NULL;
        FL->line = r->children[0]->line; 
    }
    else{
        if(noExp){
            //结构体内不能初始化
            printError(15,r->children[0]->line,"Cannot initialize field in structure.");
        }
        //TODO:增加Exp部分
        //Dec -> VarDec Assign Exp
        char* name = NULL;
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&name);
        FL->name = name;//不空
        FL->type = typePtr2;//可能变成了数组
        FL->next = NULL;
        FL->line = r->children[0]->line;
    }
    //插入变量表
    // if((searchVariableTable(FL->name) == NULL)&&(searchStructureTable(FL->name) == NULL)){
    //     //不存在重定义
    //     getVarPtr(FL->type)
    //     insertVariableTable()
    // }
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
        arrayPtr->info.array->size = r->children[2]->val.intVal;
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
    FL->line = r->children[1]->line;
    FL->next = NULL;
    //检查重名参数，注意全局作用域
    if((searchStructureTable(FL->name) != NULL)||(searchVariableTable(FL->name) != NULL)){
        //变量(形参)和变量、结构体重名
        printError(3,FL->line,"Variable redefined.");
    }
    else{
        struct Variable* varPtr = getVarPtr(FL,FL->line);
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
        handleExp(r->children[0]);
    }
    else if(r->numChildren == 3){
        //Stmt -> Return Exp Semi
        //判断返回值类型
        struct Type * expTypePtr = handleExp(r->children[1]);
        checkTypeSame(expTypePtr,typePtr);
    }
    else if(r->numChildren == 5){
        if(r->children[0]->type == Node_IF){
            //Stmt -> If LP Exp RP Stmt
            struct Type * expTypePtr = handleExp(r->children[2]);
            //TODO 判断exp的类型是不是INT

            handleStmt(r->children[4],typePtr);//返回值留给Stmt判断
        }
        else{
            //和IF 一样的......
            //Stmt -> while LP Exp RP Stmt
            struct Type * expTypePtr = handleExp(r->children[2]);
            //TODO 判断exp的类型是不是INT

            handleStmt(r->children[4],typePtr);//返回值留给Stmt判断
        }
    }
    else{
        //Stmt -> If LP Exp RP Stmt else Stmt
        struct Type * expTypePtr = handleExp(r->children[2]);
        //TODO 判断exp的类型是不是INT

        handleStmt(r->children[4],typePtr);//返回值留给Stmt判断
        handleStmt(r->children[6],typePtr);//返回值留给Stmt判断
    }
}

struct Type * handleExp(struct TreeNode* r){
    assert(r->type == Node_Exp);
    if(r->numChildren == 1){
        if(r->children[0]->type == Node_ID){
            //Exp -> ID
            //查找变量表
            struct Variable * varPtr = searchVariableTable(r->children[0]->idName);
            if(varPtr == NULL){
                //没有这个变量
                printError(1,r->children[0]->line,"Variable Undefined.");
                return NULL;
            }
            else{
                return varPtr->varType;
            }
        }
        else if(r->children[0]->type == Node_INT){
            //Exp -> INT
            //创建一个Type*
            struct Type * typePtr = (struct Type *)malloc(sizeof(struct Type));
            typePtr->kind = BASIC;
            typePtr->info.basicType = TYPE_INT;
            return typePtr;
        }
        else{
            //Exp -> FLAOT
            //创建一个Type*
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
            struct Type * typePtr = handleExp(r->children[1]);
            if((typePtr == NULL) || (typePtr->kind != BASIC) ){
                //操作数类型不匹配
                printError(7,r->children[0]->line,"Variable(s) type mismatch.");
                return NULL;
            }
            else{
                return typePtr;//类型不变
            }
        }
        else{
            //Exp -> Neg Exp
            //判断Exp 是不是NULL或者basic类型
            struct Type * typePtr = handleExp(r->children[1]);
            if((typePtr == NULL) || (typePtr->kind != BASIC) ){
                //操作数类型不匹配
                printError(7,r->children[0]->line,"Variable(s) type mismatch.");
                return NULL;
            }
            else{
                return typePtr;//类型不变
            }
        }
    }
    else if(r->numChildren == 3){
        if(r->children[1]->type == Node_ASSIGNOP){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            if(checkTypeSame(typePtr1,typePtr2)){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
            
        }
        else if(r->children[1]->type == Node_AND){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //整数才能AND
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC) && (typePtr1->info.basicType == TYPE_INT))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_OR){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //整数才能OR
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC) && (typePtr1->info.basicType == TYPE_INT))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_RELOP){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //基本类型才能RELOP
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_PLUS){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //基本类型才能PLUS
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_MINUS){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //基本类型才能Minus
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_STAR){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //基本类型才能star
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[1]->type == Node_DIV){
            //Exp -> Exp Assign Exp
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //基本类型才能DIV
            if(checkTypeSame(typePtr1,typePtr2) && ((typePtr1->kind == BASIC))){
                return typePtr1;
            }
            else{
                return NULL;//不能赋值就返回NULL
            }
        }
        else if(r->children[0]->type == Node_LP){
            //Exp -> LP Exp RP
           return handleExp(r->children[1]);
        }
        else if(r->children[1]->type == Node_LP){
            //Exp -> ID LP RP
            //函数调用
            struct Func * funcPtr = searchFuncTable(r->children[0]->idName);
            if(funcPtr == NULL){
                //调用未定义的函数
                printError(2,r->children[0]->line,"Function Undefined.");
                return NULL;
            }
            else{
                //函数定义了，判断参数是否符合
                if(checkFieldListSame(funcPtr->params,NULL)){
                    return funcPtr->retType;//返回的是函数返回类型
                }
                else{
                    printError(9,r->children[0]->type,"Function call parameters mismatch,");
                    return NULL;
                }
            }
        }
        else{
            //Exp -> Exp DOT ID
            assert(r->children[1]->type == Node_DOT);//前面太多了，assert一下
            struct Type * expTypePtr = handleExp(r->children[0]);
            //判断ptr是否非NULL且为Structure并且有域ID
            if((expTypePtr != NULL)&&(expTypePtr->kind == STRUCTURE)){
                struct FieldList * FL = expTypePtr->info.structure->structureInfo;//结构体的域
                while ((FL != NULL) && (strcmp(FL->name,r->children[2]->idName) != 0))
                {
                    FL = FL -> next;
                }
                if(FL == NULL){
                    //没有这个域
                    return NULL;
                }
                else{
                    return FL->type;
                }
            }
            else{
                //Exp的类型不是结构体
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
                //调用未定义的函数
                printError(2,r->children[0]->line,"Function Undefined.");
                return NULL;
            }
            else{
                //函数定义了，判断参数是否符合
                //先得到Args:FL,只有type重要,name不需要
                struct FieldList* FL = handleArgs(r->children[2]);
                if(checkFieldListSame(funcPtr->params,FL)){
                    return funcPtr->retType;//返回的是函数返回类型
                }
                else{
                    printError(9,r->children[0]->type,"Function call parameters mismatch,");
                    return NULL;
                }
            }
        }
        else{
            //Exp -> Exp LB Exp RB
            assert(r->children[0]->type == Node_Exp);
            struct Type * typePtr1 = handleExp(r->children[0]);
            struct Type * typePtr2 = handleExp(r->children[2]);
            //1应该是array,2是INT
            if((typePtr1 != NULL)&&(typePtr1->kind==ARRAY)&&(typePtr2 != NULL)
                &&(typePtr2->kind==BASIC)&&(typePtr2->info.basicType == TYPE_INT)){
                    return typePtr1->info.array->elem;//返回上一层类型
                }
            else{
                return NULL;
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
        FL->type = handleExp(r->children[0]);
        FL->line = r->children[0]->line;
        FL->next = NULL;
        return FL;
    }
    else{
        //Args -> Exp Comma Args
        struct FieldList* FL1 = (struct FieldList*)malloc(sizeof(struct FieldList));
        FL1->type =  handleExp(r->children[0]);//单个
        FL1->name = NULL;
        FL1->line = r->children[1]->line;
        struct FieldList* FL2 = handleArgs(r->children[2]);
        FL1->next = FL2;
        return FL1;//连上
    }
}