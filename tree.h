#ifndef TREE_H
#define TREE_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
//树节点的type的定义
//词法单元
#define Node_INT 1
#define Node_FLOAT 2
#define Node_ID 3
#define Node_SEMI 4 
#define Node_COMMA 5
#define Node_ASSIGNOP 6 
#define Node_RELOP 7
#define Node_PLUS 8
#define Node_STAR 9
#define Node_DIV 10
#define Node_AND 11
#define Node_OR 12
#define Node_DOT 13
#define Node_NOT 14
#define Node_TYPE 15
#define Node_LP 16
#define Node_RP 17
#define Node_LB 18
#define Node_RB 19
#define Node_LC 20
#define Node_RC 21
#define Node_STRUCT 22
#define Node_RETURN 23
#define Node_IF 24
#define Node_ELSE 25
#define Node_WHILE 26
#define Node_NEGETIVE 27
#define Node_MINUS 28

//语法单元
#define Node_Program 29
#define Node_ExtDefList 30
#define Node_ExtDef 31
#define Node_ExtDecList 32
#define Node_Sepcifier 33
#define Node_StructSpecifier 34
#define Node_OptTag 35
#define Node_Tag 36
#define Node_VarDec 37
#define Node_FuncDec 38
#define Node_VarList 39
#define Node_ParamDec 40
#define Node_Compst 41
#define Node_StmtList 42
#define Node_Stmt 43
#define Node_DefList 44
#define Node_Def 45
#define Node_DecList 46
#define Node_Dec 47
#define Node_Exp 48
#define Node_Args 49

//用于区分type:int float
#define TYPE_INT 50
#define TYPE_FLOAT 51


//对于一个语法单元，如果他是词法单元int id float，还有数值信息
union TreeVal
{
    char* idName;//ID
    int intVal;
    float floatVal;
    int typeVal;//int 和 float区分
};

//多叉树的实现
struct TreeNode
{
    int type;//节点类型:终结符或者非终结符,具体到id,int,expr...
    union TreeVal val;
    int numChildren;//子女数量
    struct TreeNode** children;//需要动态分配
    int line;//第一次出现的行号，依赖于children[0]的line
};

//判断r是否为词法单元
int isLexicalUnit(struct TreeNode*r);

//打印若干空格
void printSpace(int num);

//传入若干个TreeNode*，插入到parent中
struct TreeNode* insert(int type,int n,...);

//先序遍历
void preTraverse(struct TreeNode*r,int spaceNum);

extern struct TreeNode* root;//全局变量，语法树的根 

#endif