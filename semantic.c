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

struct FieldList* handleDefList(struct TreeNode* r,int isInStruc){
    assert(r->type == Node_DefList);
    if(r->numChildren == 0){
        //DefLst -> epsilon
        return NULL;
    }
    else{
        //DefLst -> Def DefList
        struct FieldList* FL1 = handleDef(r->children[0],isInStruc);//数量>=1
        struct FieldList* FL2 = handleDefList(r->children[1],isInStruc);//可能为空
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

struct FieldList* handleDef(struct TreeNode* r,int isInStruc){
    assert(r->type == Node_Def);
    //Def -> Specifier DecList Semi
    struct Type * typePtr = handleSpecifier(r->children[0]);
    return handleDecList(r->children[1],typePtr,isInStruc);
}

struct FieldList* handleDecList(struct TreeNode* r,struct Type * typePtr,int isInStruc){
    assert(r->type == Node_DecList);
    if(r->numChildren == 1){
        //DecList -> Dec
        return handleDec(r->children[0],typePtr,isInStruc);
    }
    else{
        //DecList -> Dec comma Declist
        struct FieldList* FL1 = handleDec(r->children[0],typePtr,isInStruc);
        struct FieldList* FL2 = handleDecList(r->children[2],typePtr,isInStruc);
        FL1->next = FL2;
        return FL1;
    }
}

struct FieldList* handleDec(struct TreeNode* r,struct Type * typePtr,int isInStruc){
    assert(r->type == Node_Dec);
    if(r->numChildren == 1){
        //Dec -> VarDec
        char* name = NULL;
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&name);
        struct FieldList* FL = (struct FieldList*)malloc(sizeof(struct FieldList));
        FL->name = name;//不空
        FL->type = typePtr2;//可能变成了数组
        FL->next = NULL;
        FL->line = r->children[0]->line;
        return FL;
    }
    else{
        if(isInStruc){
            //结构体内不能初始化
            printError(15,r->children[0]->line,"Cannot initialize field in structure.");
        }
        //TODO:增加Exp部分
        //Dec -> VarDec Assign Exp
        char* name = NULL;
        struct Type* typePtr2 = handleVarDec(r->children[0],typePtr,&name);
        struct FieldList* FL = (struct FieldList*)malloc(sizeof(struct FieldList));
        FL->name = name;//不空
        FL->type = typePtr2;//可能变成了数组
        FL->next = NULL;
        FL->line = r->children[0]->line;
        return FL;
    }
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