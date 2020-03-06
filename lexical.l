%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include"syntax.tab.h"
    #include "tree.h"
    void preSetYylval(int type);
%}
digit [0-9]
digits {digit}+
digit8 [0-7]
digit16 [0-9A-Fa-f]
letter [a-zA-Z]
_letter {letter}|(_)
_letterDigit {_letter}|{digit}
whiteSpace [ \t\n\r]
relop ("<")|("<=")|(">")|(">=")|("==")|("!=")
decimal 0|([1-9]({digit}*))
octal 0({digit8}+)
hexical (0[Xx])({digit16}+)
float ({digits})(\.)({digits})
id {_letter}(({_letter}|{digit})*)
%option yylineno
%%
{whiteSpace}+ {
}
";" {
    preSetYylval(Node_SEMI);
    return SEMI;
}
"," {
    preSetYylval(Node_COMMA);
    return COMMA;
}
{relop} {
    preSetYylval(Node_RELOP);
    return RELOP;
}
"=" {
    preSetYylval(Node_ASSIGNOP);
    return ASSIGNOP;
}
"+" {
    preSetYylval(Node_PLUS);
    return PLUS;
}
"-" {
    preSetYylval(Node_MINUS);
    return MINUS;
}
"*" {
    preSetYylval(Node_STAR);
    return STAR;
}
"/" {
    preSetYylval(Node_DIV);
    return DIV;
}
"&&" {
    preSetYylval(Node_AND);
    return AND;
}
"||" {
    preSetYylval(Node_OR);
    return OR;
}
"!" {
    preSetYylval(Node_NOT);
    return NOT;
}
"." {
    preSetYylval(Node_DOT);
    return DOT;
}
(int) {
    preSetYylval(Node_TYPE);
    yylval.node->val.typeVal = TYPE_INT;
    return TYPE;
}
(float) {
    preSetYylval(Node_TYPE);
    yylval.node->val.typeVal = TYPE_FLOAT;
    return TYPE;
}
"(" {
    preSetYylval(Node_LP);
    return LP;
}
")" {
    preSetYylval(Node_RP);
    return RP;
}
"[" {
    preSetYylval(Node_LB);
    return LB;
}
"]" {
    preSetYylval(Node_RB);
    return RB;
}
"{" {
    preSetYylval(Node_LC);
    return LC;
}
"}" {
    preSetYylval(Node_RC);
    return RC;
}
"struct" {
    preSetYylval(Node_STRUCT);
    return STRUCT;
}
"return" {
    preSetYylval(Node_RETURN);
    return RETURN;
}
"if" {
    preSetYylval(Node_IF);
    return IF;
}
"else" {
    preSetYylval(Node_ELSE);
    return ELSE;
}
"while" {
    preSetYylval(Node_WHILE);
    return WHILE;
}
{float} {
    preSetYylval(Node_FLOAT);
    yylval.node->val.floatVal = atof(yytext);
    return FLOAT;
}
{decimal} {
    preSetYylval(Node_INT);
    yylval.node->val.intVal = atoi(yytext);
    return INT;
}
{octal} {
    preSetYylval(Node_INT);
    char* ptr;
    yylval.node->val.intVal = (int)strtol(yytext,&ptr,8);
    return INT;
}
{hexical} {
    preSetYylval(Node_INT);
    char* ptr;
    yylval.node->val.intVal = (int)strtol(yytext,&ptr,16);
    return INT;
}
{id} {
    preSetYylval(Node_ID);
    yylval.node->val.idName = (char*)malloc(sizeof(char)*(yyleng+1));
    memcpy(yylval.node->val.idName,yytext,sizeof(char)*(yyleng+1));
    return ID;
}
. {
    printf("Error type A at line %d:Mysterious character \'%c\'\n",yylineno,*yytext);
}
%%
void preSetYylval(int type)
{
    yylval.node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	yylval.node->type = type;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
}