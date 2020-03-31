#include "semantic.h"
#include "tree.h"
#include <assert.h>
#include <string.h>

struct Variable *variableTable[TABLE_SIZE];
struct Structure *structureTable[TABLE_SIZE];
struct Func *funcTable[TABLE_SIZE];

void printError(int type,int line,char* msg){
    printf("Error type %d at line %d:%s\n",type,line,msg);
}

unsigned hash(char *name)
{
    unsigned val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~TABLE_SIZE)
            val = (val ^ (i >> 12)) & TABLE_SIZE;
    }
    return val;
}

void insertFuncTable(struct Func* func){
    assert(func != NULL);
    assert(func->name != NULL);
    unsigned idx = hash(func->name);
    struct Func* head = funcTable[idx];
    //头部插入
    func->next = head;
    funcTable[idx] = func;
}

void insertStructureTable(struct Structure*structure){
    assert(structure != NULL);
    assert(structure->name != NULL);
    unsigned idx = hash(structure->name);
    struct Structure* head = structureTable[idx];
    //头部插入
    structure->next = head;
    structureTable[idx] = structure;
}

void insertVariableTable(struct Variable*variable){
    assert(variable != NULL);
    assert(variable->name != NULL);
    unsigned idx = hash(variable->name);
    struct Variable* head = variableTable[idx];
    //头部插入
    variable->next = head;
    variableTable[idx] = variable;
}

struct Func* searchFuncTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name);
    struct Func* head = funcTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}

struct Structure* searchStructureTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name);
    struct Structure* head = structureTable[idx];
    while (head != NULL && (strcmp(name,head->name) != 0))
    {
        head = head->next;
    }
    return head;
}

struct Variable* searchVariableTable(char*name){
    assert(name != NULL);
    unsigned idx = hash(name);
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
    if(r->numChildren == 2){//Extdef -> Specifier SEMI,对应于结构体定义
        handleSpecifier(r->children[0]);
        printType(r->children[0]->synAttr.type,0);
    }
    else if(r->children[2]->type == Node_SEMI){//Extdef -> Specifier ExtDecList SEMI,对应于全局变量定义
        handleSpecifier(r->children[0]);//这时候Specifier的类型信息已经存在synAttr中,synAttr作为ExtDecList的继承属性
        //设置ExtDecList的inhAttr,在ExtDecList的解析中完成变量表的修改
        //通过继承属性传下去
        r->children[1]->inhAttr.type = r->children[0]->synAttr.type;
        handleExtDecList(r->children[1]);
        printType(r->children[1]->synAttr.defList->type,0);
    }
    else if(r->children[2]->type == Node_Compst){//函数定义

    }
}

void handleExtDecList(struct TreeNode* r){
    //类比DecList应该没问题吧???
    assert(r->type == Node_ExtDecList);
    //全局变量的声明
    if(r->numChildren == 1){//ExtDecList -> VarDec
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleVarDec(r->children[0]);//varDec利用继承的类型确定类型信息,返回一个FieldList，仅包含一个节点
        r->synAttr.defList = r->children[0]->synAttr.defList; 
    }
    else{//ExtDecList -> VarDec Comma ExtDecList
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        r->children[2]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleVarDec(r->children[0]);
        handleExtDecList(r->children[2]);
        r->children[0]->synAttr.defList->next = r->children[2]->synAttr.defList;//连上了
        r->synAttr.defList = r->children[0]->synAttr.defList; 
    }
}

void handleSpecifier(struct TreeNode* r){
    assert(r->type == Node_Sepcifier);
    if(r->children[0]->type == Node_TYPE){//Specifier -> TYPE 基本类型
        //直接修改r的综合属性synAttr为类型：基本类型
        r->synAttr.type = (struct Type*)malloc(sizeof(struct Type));
        r->synAttr.type->kind = BASIC;//基本类型
        r->synAttr.type->info.basicType = r->children[0]->val.typeVal;//TYPE_INT或者TYPE_FLOAT
    }
    else if(r->children[0]->type == Node_StructSpecifier){
        //综合属性为类型：结构体类型
        //handleStructSpecifier有自己的synAttr,r->snAttr直接引用它就好了(指针赋值)
        handleStructSpecifier(r->children[0]);
        //现在strcuSpecifier的type已经准备好了,空间分配好了
        //structSpecifier的综合属性是类型
        r->synAttr.type = r->children[0]->synAttr.type;
    }
    else{
        assert(0);
    }
}

void handleStructSpecifier(struct TreeNode* r){
    assert(r->type == Node_StructSpecifier);
    if(r->numChildren == 2){//strcuSpecifier->struct tag
        //tag->id,tag的idname就是id的idname
        handleTag(r->children[1]);
        struct Structure* strucPtr = searchStructureTable(r->children[1]->idName);
        //根据tag->idName查找strcuTable，如果找不到报错
        if(strucPtr == NULL){
            //没有这个名字的结构体
            printError(17,r->children[1]->line,"Undefined Structre");            
        }
        else{
            r->synAttr.type = (struct Type*)malloc(sizeof(struct Type));
            r->synAttr.type->kind = STRUCTURE;
            //类型的具体信息:是结构,也就是一个结构体表的表项(next指针并不重要)
            r->synAttr.type->info.structure = strucPtr;
        }
    }
    else{
        //structSpecifier -> struct OpaTag LC DefList RC
        //optTag -> ID | epsilon
        handleOptTag(r->children[1]);//optag = epsilon(根据optTag的name是否为null)则往结构体表插入(检查重名)，否则不插入。
        //但是r的综合属性:结构体类型还是要写的
        handleDefList(r->children[3]);
        //children[3]的综合属性应该是一个FieldLList
        r->synAttr.type = (struct Type*)malloc(sizeof(struct Type));
        r->synAttr.type->kind = STRUCTURE;
        r->synAttr.type->info.structure = (struct Structure *)malloc(sizeof(struct Structure));
        r->synAttr.type->info.structure->structureInfo = r->children[3]->synAttr.defList;//defList自己分配了空间
        r->synAttr.type->info.structure->name = r->children[1]->idName;
        r->synAttr.type->info.structure->next = NULL;
        //根据optTag是否有名决定插入结构体表
        if(r->children[1]->idName != NULL){
            //有名
            insertStructureTable(r->synAttr.type->info.structure);
        }
    }
}

void handleTag(struct TreeNode* r){
    assert(r->type == Node_Tag);
    r->idName = r->children[0]->idName;//名字直接复制就好了,其实这个名字应该是Tag的综合属性，应该放在synAttr中的
}

void handleOptTag(struct TreeNode* r){
    assert(r->type == Node_OptTag);
    if(r->numChildren == 0){
        //OptTag->epsilon
        r->idName = NULL;
    }
    else{
        r->idName = r->children[0]->idName;
    }
}

void handleDefList(struct TreeNode* r){
    assert(r->type == Node_DefList);
    if(r->numChildren == 0){
        r->synAttr.defList = NULL;
    }
    else{        
        handleDef(r->children[0]);//children[0]的综合属性是FieldList，可能有多个节点,至少一个
        handleDefList(r->children[1]);//是一串FieldList,可以 为NULL
        //采用头部插入的方式，但是对于int a,b;的情况，child[0]的综合属性可能有多个节点，找最后一个
        struct FieldList * tmp = r->children[0]->synAttr.defList;
        //TODO modify 这样有点低效，后期考虑效率，需要修改数据结构
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        // r->children[0]->synAttr.defList->next = r->children[1]->synAttr.defList;
        // r->synAttr.defList = r->children[0]->synAttr.defList;//连上了
        tmp->next = r->children[1]->synAttr.defList;
        r->synAttr.defList = r->children[0]->synAttr.defList;
    }
}

void handleDef(struct TreeNode* r){
    assert(r->type == Node_Def);
    //Def-> Specifier DecList SEMI
    //先得到类型
    handleSpecifier(r->children[0]);//这是children[0]的综合属性:类型已经确定了
    //设置继承属性,因为decList可能有初始化语句
    r->children[1]->inhAttr.type = r->children[0]->synAttr.type;//类型指针直接复制就行了
    handleDecList(r->children[1]);//这时候DecList的FieldList已经写了并且每个节点的类型也确定了
    //TODO
    r->synAttr.defList = r->children[1]->synAttr.defList;
}

void handleDecList(struct TreeNode* r){
    assert(r->type == Node_DecList);
    //r的继承属性已经得到了，是Specifier的类型Type
    if(r->numChildren == 1){//DecList -> Dec
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleDec(r->children[0]);//varDec利用继承的类型确定类型信息,返回一个FieldList，仅包含一个节点
        r->synAttr.defList = r->children[0]->synAttr.defList; 
    }
    else{//DecList -> Dec comma DecList
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        r->children[2]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleDec(r->children[0]);
        handleDecList(r->children[2]);
        r->children[0]->synAttr.defList->next = r->children[2]->synAttr.defList;//连上了
        r->synAttr.defList = r->children[0]->synAttr.defList; 
    }
}

void handleDec(struct TreeNode* r){
    assert(r->type == Node_Dec);
    if(r->numChildren == 1){//Dec -> VarDec
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleVarDec(r->children[0]);//返回一个FieldList，只有一个节点
        r->synAttr.defList = r->children[0]->synAttr.defList; 
    }
    else{//Dec -> VarDec Assign Exp
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleVarDec(r->children[0]);//返回一个FieldList，只有一个节点
        r->synAttr.defList = r->children[0]->synAttr.defList; 
        //TODO handle Exp,判断Exp的类型是否和inhAttr相容
    }
}

void handleVarDec(struct TreeNode* r){
    assert(r->type == Node_VarDec);
    if(r->numChildren == 1){//VarDec -> ID
        r->synAttr.defList = (struct FieldList *)malloc(sizeof(struct FieldList));
        r->synAttr.defList->name = r->children[0]->idName;//就是这个域的名字
        r->synAttr.defList->type = r->inhAttr.type;//继承的类型
        r->synAttr.defList->next = NULL;
    }
    else{//VarDec -> VarDec1 LB INT RB
        r->children[0]->inhAttr.type = r->inhAttr.type;//类型继承下去
        handleVarDec(r->children[0]);
        r->synAttr.defList = (struct FieldList *)malloc(sizeof(struct FieldList));
        r->synAttr.defList->name = r->children[0]->synAttr.defList->name;//就是这个域的名字int a[2][3]的"a"
        r->synAttr.defList->type = (struct Type *)malloc(sizeof(struct Type));
        r->synAttr.defList->type->kind = ARRAY;
        r->synAttr.defList->type->info.array = (struct Array *)malloc(sizeof(struct Array));
        r->synAttr.defList->type->info.array->elem = r->children[0]->synAttr.defList->type;
        assert(r->children[2]->type == Node_INT);
        r->synAttr.defList->type->info.array->size = r->children[2]->val.intVal;//int a[2][3]
        //Attention!这里数组各维大小我其实是写反的，但是好像问题不大?
        r->synAttr.defList->next = NULL;
    }
}