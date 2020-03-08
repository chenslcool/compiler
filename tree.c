#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
struct TreeNode *root;

//将n个子女节点插入到parent中，修改parent的line,numChildrean和Children
struct TreeNode *insert(int type, int n, ...)
{
    // printf("insert\n");
    struct TreeNode *parent = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    parent->numChildren = n;
    parent->type = type;
    if (n > 0)
    {
        // printf("insert:root=%d, parent=%d, n=%d, parent.type=%d, ",(int)root,(int)parent,n,type);
        parent->children = (struct TreeNode **)malloc(sizeof(struct TreeNode *) * n);
        struct TreeNode *tmp = NULL;
        va_list vl;
        va_start(vl, n);
        // printf("chaild: ");
        for (int i = 0; i < n; ++i)
        {
            tmp = va_arg(vl, struct TreeNode *);
            // printf("add = %d, type=%d ",(int)tmp,tmp->type);
            if (i == 0)
            {
                parent->line = tmp->line;
            }
            parent->children[i] = tmp;
        }
        va_end(vl);
        // printf("\n");
    }
    return parent;
}

int isLexicalUnit(struct TreeNode *r)
{
    int type = r->type;
    //判断type是不是词法单元，全靠之前词法单元和语法单元define的顺序
    return (type >= Node_INT) && (type <= Node_MINUS);
}

void preTraverse(struct TreeNode *r, int spaceNum)
{
    //先序遍历,spaceNum为空格数
    if (r != NULL)
    {
        
        if (isLexicalUnit(r))
        {
            printSpace(spaceNum);
            switch (r->type)
            {
            case Node_ID:
            {
                printf("ID: %s", r->val.idName);
            }
            break;
            case Node_TYPE:
            {
                printf("TYPE: ");
                if (r->val.typeVal == TYPE_INT)
                {
                    printf("int");
                }
                else
                {
                    printf("float");
                }
            }
            break;
            case Node_INT:
            {
                printf("INT: %d", r->val.intVal);
            }
            break;
            case Node_FLOAT:
            {
                printf("FLOAT: %f", r->val.floatVal);
            }
            break;
            case Node_SEMI:
            {
                printf("SEMI");
            }
            break;
            case Node_COMMA:
            {
                printf("COMMA");
            }
            break;
            case Node_ASSIGNOP:
            {
                printf("ASSIGNOP");
            }
            break;
            case Node_RELOP:
            {
                printf("RELOP");
            }
            break;
            case Node_PLUS:
            {
                printf("PLUS");
            }
            break;
            case Node_STAR:
            {
                printf("STAR");
            }
            break;
            case Node_DIV:
            {
                printf("DIV");
            }
            break;
            case Node_AND:
            {
                printf("AND");
            }
            break;
            case Node_OR:
            {
                printf("OR");
            }
            break;
            case Node_DOT:
            {
                printf("DOT");
            }
            break;
            case Node_NOT:
            {
                printf("NOT");
            }
            break;
            case Node_LP:
            {
                printf("LP");
            }
            break;
            case Node_RP:
            {
                printf("RP");
            }
            break;
            case Node_LB:
            {
                printf("LB");
            }
            break;
            case Node_RB:
            {
                printf("RB");
            }
            break;
            case Node_LC:
            {
                printf("LC");
            }
            break;
            case Node_RC:
            {
                printf("RC");
            }
            break;
            case Node_STRUCT:
            {
                printf("STRUCT");
            }
            break;
            case Node_RETURN:
            {
                printf("RETURN");
            }
            break;
            case Node_IF:
            {
                printf("IF");
            }
            break;
            case Node_ELSE:
            {
                printf("ELSE");
            }
            break;
            case Node_WHILE:
            {
                printf("WHILE");
            }
            break;
            case Node_MINUS:
            {
                printf("MINUS");
            }
            break;
            }
            printf("\n");
        }
        else
        {
            //是语法单元
            if (r->numChildren > 0)
            {
                //非空
                //先根
                // printf("switch: r->type=%d\n\n\n", r->type);
                printSpace(spaceNum);
                switch (r->type)
                {
                case Node_Program:
                {
                    printf("Program (%d)", r->line);
                }
                break;
                case Node_ExtDefList:
                {
                    printf("ExtDefList (%d)", r->line);
                }
                break;
                case Node_ExtDef:
                {
                    printf("ExtDef (%d)", r->line);
                }
                break;
                case Node_ExtDecList:
                {
                    printf("ExtDecList (%d)", r->line);
                }
                break;
                case Node_Sepcifier:
                {
                    printf("Sepcifier (%d)", r->line);
                }
                break;
                case Node_StructSpecifier:
                {
                    printf("StructSpecifier (%d)", r->line);
                }
                break;
                case Node_OptTag:
                {
                    printf("OptTag (%d)", r->line);
                }
                break;
                case Node_Tag:
                {
                    printf("Tag (%d)", r->line);
                }
                break;
                case Node_VarDec:
                {
                    printf("VarDec (%d)", r->line);
                }
                break;
                case Node_FuncDec:
                {
                    printf("FuncDec (%d)", r->line);
                }
                break;
                case Node_VarList:
                {
                    printf("VarList (%d)", r->line);
                }
                break;
                case Node_ParamDec:
                {
                    printf("ParamDec (%d)", r->line);
                }
                break;
                case Node_Compst:
                {
                    printf("Compst (%d)", r->line);
                }
                break;
                case Node_StmtList:
                {
                    printf("StmtList (%d)", r->line);
                }
                break;
                case Node_Stmt:
                {
                    printf("Stmt (%d)", r->line);
                }
                break;
                case Node_DefList:
                {
                    printf("DefList (%d)", r->line);
                }
                break;
                case Node_Def:
                {
                    printf("Def (%d)", r->line);
                }
                break;
                case Node_DecList:
                {
                    printf("DecList (%d)", r->line);
                }
                break;
                case Node_Dec:
                {
                    printf("Dec (%d)", r->line);
                }
                break;
                case Node_Exp:
                {
                    printf("Exp (%d)", r->line);
                }
                break;
                case Node_Args:
                {
                    printf("Args (%d)", r->line);
                }
                break;
                }
                printf("\n");
                for (int i = 0; i < r->numChildren; ++i)
                {
                    preTraverse(r->children[i], spaceNum + 2);
                }
            }
        }
        // printf("\n");
    }
}

void printSpace(int num)
{
    for (int i = 0; i < num; ++i)
    {
        printf(" ");
    }
}