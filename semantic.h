#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "tree.h"
//哈希表大小
#define TABLE_SIZE 100


//类型定义:基本类型、数组类型、结构体
struct Type
{
    enum {
        BASIC,
        ARRAY,
        STRUCTURE
    }kind;//类别
    union 
    {
        int basicType;//int or float,在tree.h 的 treeVal中定义
        struct Array* array;//数组
        struct Structure* structure;//结构体
    }info;//具体类型信息和kind一一对应
};

//类型列表：用于描述结构体成员、函数形参列表
struct FieldList
{
    char* name;//结构体成员名字，如果是函数形参就无所谓
    struct Type* type;//类型
    struct FieldList* next;//链表指针
};

//函数类型
struct Func
{
    char* name;//函数名 
    struct Type* retType;//返回值类型
    struct FieldList* params;//形参列表，参数的name并不重要
    struct Func* next;//在表项中的下一个
};

//数组类型
struct Array
{
    struct Type* elem;
    int size;
};

//变量表项
struct Variable
{
    char* name;//变量名
    struct Type* varType;
    int firstAppearanceLine;//第一次出现的行号
    struct Variable* next; 
};

//结构体表项:仅包含有名结构体
struct Structure
{
    char* name;
    struct FieldList* structureInfo;//和变量valType相同类型是为了便于之后类型的比较
    struct Structure* next;//这个next指针只在结构体表项中有用
};


//根据name查表
struct Func* searchFuncTable(char*name);
struct Structure* searchStructureTable(char*name);
struct Variable* searchVariableTable(char*name); 

//插入表项,名字在结构体中已经有了
void insertFuncTable(struct Func* func);
void insertStructureTable(struct Structure*structure);
void insertVariableTable(struct Variable*variable);

//打印类型信息，便于调试
void printType(struct Type*,int nrSpace);

//根据name取得哈希值，得到table表项地址
unsigned hash(char*name);

void printError(int type,int line,char* msg);

void handleProgram(struct TreeNode* r);

void handleExtDef(struct TreeNode* r);

void handleExtDefList(struct TreeNode* r);

void handleSpecifier(struct TreeNode* r);

void handleStructSpecifier(struct TreeNode* r);

void handleTag(struct TreeNode* r);

void handleOptTag(struct TreeNode* r);

void handleDefList(struct TreeNode* r);

void handleDef(struct TreeNode* r);

void handleDecList(struct TreeNode* r);

void handleDec(struct TreeNode* r);

void handleVarDec(struct TreeNode* r);

#endif