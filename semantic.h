#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "tree.h"
//哈希表大小
#define TABLE_SIZE 100
#define INT_FLOAT_SIZE 4
extern int numHashSearch;

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
    int line;//第一次出现的行
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
    //元素类型
    struct Type* elem;
    //元素数量
    int numElem;
    //单个元素所占空间
    int elemWidth;
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

//用于判断FL*中是否有重复名字的hashset表项
struct NameNode
{
    char* name;
    struct NameNode*next;
};

struct Operand
{
    //操作数类型
    enum{
        OPERAND_LABEL,//跳转标号
        OPERAND_FUNC,//函数名
        OPEARND_VAR,//普通局部变量名
        OPEARND_TMP_VAR,//中间代码临时变量 tmpNo
        OPEARND_CONSTANT,//常数INT
        OPEARND_ADDR//地址，其实还是用一个临时变量存储地址 tmpNo
    }kind;
    union{
        int laeblNo;
        char* funcName;
        char* varName;
        int tmpVarNo;//临时变量ID或者临时变量地址
        int constantVal;
    }info;
};

//中间代码
struct InterCode
{
    enum{
        IC_LABEL_DEF,
        IC_FUNC_DEF,
        IC_ASSIGN,
        IC_PLUS,
        IC_SUB,
        IC_MUL,
        IC_DIV,
        IC_GET_ADDR,
        IC_WRITE_TO_ADDR,
        IC_DOTO,
        IC_RELOP_GOTO,
        IC_RETURN,
        IC_DEC,
        IC_ARG,
        IC_CALL,
        IC_PARAM,
        IC_READ,
        IC_WRITE
    }kind;
    struct Operand* operands[3];//最多3个操作数
    int Relop;//具体表明relop类型
    int numOperands;//操作数数量
    struct InterCode* next;
    struct InterCode* prev;
};


struct NameNode** getNameHashSet(int sz);
int nameSetContains(struct NameNode** set,int sz,char* name);
void insertIntoSet(struct NameNode** set,int sz,char*name);
void freeSet(struct NameNode** set,int sz);

//根据name查表
struct Func* searchFuncTable(char*name); 
struct Structure* searchStructureTable(char*name);
struct Variable* searchVariableTable(char*name); 

//插入表项,名字在结构体中已经有了
void insertFuncTable(struct Func* func);
void insertStructureTable(struct Structure*structure);
void insertVariableTable(struct Variable*variable);

//根据一个只有一个节点的创建出一个变量符号，用于插入变量表
struct Variable* getVarPtr(struct FieldList*FL,int line);

//打印类型信息，便于调试
void printType(struct Type*,int nrSpace);

//打印一系列域
void printFieldList(struct FieldList* ptr,int nrSpace);

//打印函数头
void printFuncDec(struct Func* funcPtr,int nrSpace);

//计算类型所占的空间
int calculateWidth(struct Type* typePtr);

//根据name取得哈希值，得到table表项地址
unsigned hash(char*name,int sz);

void printError(int type,int line,char* msg);

//检查是否有相同的名字
void checkNoDuplicateName(struct FieldList*FL);

//判断两个类型是不是一致
int checkTypeSame(struct Type *typePtr1,struct Type *typePtr2);

//判断参数列表是否符合
int checkFieldListSame(struct FieldList*FL1,struct FieldList*FL2);

//反转数组,并且填写width
void reverseArray(struct Type * arrayType);

void handleProgram(struct TreeNode* r);

void handleExtDef(struct TreeNode* r);

void handleExtDefList(struct TreeNode* r);

struct Type* handleSpecifier(struct TreeNode* r);

struct FieldList* handleExtDecList(struct TreeNode* r,struct Type* typrPtr);

struct Type* handleStructSpecifier(struct TreeNode* r);

char* handleTag(struct TreeNode* r);

char* handleOptTag(struct TreeNode* r);

struct FieldList* handleDefList(struct TreeNode* r,int isInStruc);

struct FieldList* handleDef(struct TreeNode* r,int isInStruc);

struct FieldList* handleDecList(struct TreeNode* r,struct Type * typtPtr,int isInStruc);

struct FieldList* handleDec(struct TreeNode* r,struct Type * typtPtr,int isInStruc);

struct Type* handleVarDec(struct TreeNode* r,struct Type * typtPtr,char**namePtr);

struct FieldList* handleVarDec2(struct TreeNode* r,struct Type * typePtr);

struct Func* handleFuncDec(struct TreeNode* r,struct Type * typePtr);

struct FieldList* handleVarList(struct TreeNode* r);

struct FieldList* handleParamDec(struct TreeNode* r);

void handleCompst(struct TreeNode* r,struct Type * typePtr);

void handleStmtList(struct TreeNode* r,struct Type * typePtr);

void handleStmt(struct TreeNode* r,struct Type * typePtr);

struct Type * handleExp(struct TreeNode* r);

struct FieldList* handleArgs(struct TreeNode* r);

void appendInterCodeToList(struct InterCode* ICNodePtr);
#endif