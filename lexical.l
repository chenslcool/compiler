%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include"syntax.tab.h"
    #include "tree.h"
    int preIsExp = 0;//前面是否是表达式(表达式以 ),],ID,INT,FLOAT终结符结束)
    void preSetYylval(int type);
    extern int SyntaxError;
    extern int containsStruct;//在词法层面检测struct的存在性(^-^)
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
badHexical {hexical}{id}
badOctal ({octal}{id})|({octal}[89]+{id}?)|(0[89]+{id}?)
badDecimal {decimal}{id}
badFloat {float}{id}
badInteger {badDecimal}|{badOctal}|{badHexical}
%option yylineno
%%
{whiteSpace}+ {
}
";" {
	preIsExp = 0;
    preSetYylval(Node_SEMI);
    return SEMI;
}
"," {
	preIsExp = 0;
    preSetYylval(Node_COMMA);
    return COMMA;
}
{relop} {
	preIsExp = 0;
    preSetYylval(Node_RELOP);
    //具体类型也要保存
    yylval.node->idName = (char*)malloc(sizeof(char)*(yyleng+1));
    memcpy(yylval.node->idName,yytext,sizeof(char)*(yyleng+1));
    return RELOP;
}
"=" {
	preIsExp = 0;
    preSetYylval(Node_ASSIGNOP);
    return ASSIGNOP;
}
"+" {
	preIsExp = 0;
    preSetYylval(Node_PLUS);
    return PLUS;
}
"-" {
    //根据前面是否为表达式区分负号和减号
	if(preIsExp == 1)
    {
        preIsExp = 0;
        preSetYylval(Node_MINUS);
        // printf("减号:line = %d",yylineno);
        return MINUS;
    }
    else{
        preIsExp = 0;
        preSetYylval(Node_NEGETIVE);
        // printf("负号:line = %d",yylineno);
        return NEGETIVE;
    }
}
"*" {
	preIsExp = 0;
    preSetYylval(Node_STAR);
    return STAR;
}
"/" {
	preIsExp = 0;
    preSetYylval(Node_DIV);
    return DIV;
}
"&&" {
	preIsExp = 0;
    preSetYylval(Node_AND);
    return AND;
}
"||" {
	preIsExp = 0;
    preSetYylval(Node_OR);
    return OR;
}
"!" {
	preIsExp = 0;
    preSetYylval(Node_NOT);
    return NOT;
}
"." {
	preIsExp = 0;
    preSetYylval(Node_DOT);
    return DOT;
}
(int) {
	preIsExp = 0;
    preSetYylval(Node_TYPE);
    yylval.node->val.typeVal = TYPE_INT;
    return TYPE;
}
(float) {
	preIsExp = 0;
    preSetYylval(Node_TYPE);
    yylval.node->val.typeVal = TYPE_FLOAT;
    return TYPE;
}
"(" {
	preIsExp = 0;
    preSetYylval(Node_LP);
    return LP;
}
")" {
	preIsExp = 1;
    preSetYylval(Node_RP);
    return RP;
}
"[" {
	preIsExp = 0;
    preSetYylval(Node_LB);
    return LB;
}
"]" {
	preIsExp = 1;
    preSetYylval(Node_RB);
    return RB;
}
"{" {
	preIsExp = 0;
    preSetYylval(Node_LC);
    return LC;
}
"}" {
	preIsExp = 0;
    preSetYylval(Node_RC);
    return RC;
}
"struct" {
    containsStruct = 1;
	preIsExp = 0;
    preSetYylval(Node_STRUCT);
    return STRUCT;
}
"return" {
	preIsExp = 0;
    preSetYylval(Node_RETURN);
    return RETURN;
}
"if" {
	preIsExp = 0;
    preSetYylval(Node_IF);
    return IF;
}
"else" {
	preIsExp = 0;
    preSetYylval(Node_ELSE);
    return ELSE;
}
"while" {
	preIsExp = 0;
    preSetYylval(Node_WHILE);
    return WHILE;
}
{float} {
	preIsExp = 1;
    preSetYylval(Node_FLOAT);
    yylval.node->val.floatVal = atof(yytext);
    return FLOAT;
}
{decimal} {
	preIsExp = 1;
    preSetYylval(Node_INT);
    yylval.node->val.intVal = atoi(yytext);
    return INT;
}
{octal} {
	preIsExp = 1;
    preSetYylval(Node_INT);
    char* ptr;
    yylval.node->val.intVal = (int)strtol(yytext,&ptr,8);
    return INT;
}
{hexical} {
	preIsExp = 1;
    preSetYylval(Node_INT);
    char* ptr;
    yylval.node->val.intVal = (int)strtol(yytext,&ptr,16);
    return INT;
}
{id} {
	preIsExp = 1;
    preSetYylval(Node_ID);
    yylval.node->idName = (char*)malloc(sizeof(char)*(yyleng+1));
    memcpy(yylval.node->idName,yytext,sizeof(char)*(yyleng+1));
    return ID;
}
. {
    SyntaxError = 1;
    printf("Error type A at line %d: Mysterious character \'%c\'.\n",yylineno,*yytext);
}
%%
void preSetYylval(int type)
{
    yylval.node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	yylval.node->type = type;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    yylval.node->lastLine = yylineno;
}