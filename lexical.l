%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #include"syntax.tab.h"
    #include "tree.h"
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng; \
        yylval.node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
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
    // printf("%d\n",yytext[0]);
    if(yytext[0] == '\n'){
        yycolumn = 1;
        //printf("got \\n\n");
    }
}

";" {
    //printf("SEMI\n");
    yylval.node->type = Node_SEMI;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return SEMI;
}

"," {
    //printf("COMMA\n");
    yylval.node->type = Node_COMMA;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return COMMA;
}

{relop} {
    //printf("RELOP:%s\n",yytext);
    yylval.node->type = Node_RELOP;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return RELOP;
}

"=" {
    //printf("ASSIGNOP\n");
    yylval.node->type = Node_ASSIGNOP;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return ASSIGNOP;
}


"+" {
    //printf("PLUS\n");
    yylval.node->type = Node_PLUS;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return PLUS;
}

"-" {
    //printf("MINUS\n");
    yylval.node->type = Node_MINUS;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return MINUS;
}

"*" {
    //printf("STAR\n");
    yylval.node->type = Node_STAR;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return STAR;
}

"/" {
    //printf("DIV\n");
    yylval.node->type = Node_DIV;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return DIV;
}

"&&" {
    //printf("AND\n");
    yylval.node->type = Node_AND;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return AND;
}

"||" {
    //printf("OR\n");
    yylval.node->type = Node_OR;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return OR;
}

"!" {
    //printf("NOT\n");
    yylval.node->type = Node_NOT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return NOT;
}

"." {
    //printf("DOT\n");
    yylval.node->type = Node_DOT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return DOT;
}

(int) {
    //printf("TYPE:%s\n",yytext);
    yylval.node->type = Node_TYPE;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    yylval.node->val.typeVal = TYPE_INT;
    return TYPE;
}

(float) {
    //printf("TYPE:%s\n",yytext);
    yylval.node->type = Node_TYPE;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    yylval.node->val.typeVal = TYPE_FLOAT;
    return TYPE;
}

"(" {
    //printf("LP\n");
    yylval.node->type = Node_LP;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return LP;
}

")" {
    //printf("RP\n");
    yylval.node->type = Node_RP;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return RP;
}

"[" {
    //printf("LB\n");
    yylval.node->type = Node_LB;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return LB;
}

"]" {
    //printf("RB\n");
    yylval.node->type = Node_RB;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return RB;
}

"{" {
    //printf("LC\n");
    yylval.node->type = Node_LC;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return LC;
}

"}" {
    //printf("RC\n");
    yylval.node->type = Node_RC;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return RC;
}

"struct" {
    //printf("STRUCT\n");
    yylval.node->type = Node_STRUCT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return STRUCT;
}

"return" {
    //printf("RETURN\n");
    yylval.node->type = Node_RETURN;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return RETURN;
}

"if" {
    //printf("IF\n");
    yylval.node->type = Node_IF;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return IF;
}

"else" {
    //printf("ELSE\n");
    yylval.node->type = Node_ELSE;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return ELSE;
}

"while" {
    //printf("WHILE\n");
    yylval.node->type = Node_WHILE;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    return WHILE;
}

{float} {
    //printf("FLOAT :%s\n",yytext);
    // yylval.type_float = atof(yytext);
    yylval.node->type = Node_FLOAT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    yylval.node->val.floatVal = atof(yytext);
    return FLOAT;
}

{decimal} {
    yylval.node->type = Node_INT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    yylval.node->val.intVal = atoi(yytext);
    // printf("INT:%s\n",yytext);
    // printf("十进制:%d\n",yylval.node->val.intVal);
    return INT;
}

{octal} {
    yylval.node->type = Node_INT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    char* ptr;
    yylval.node->val.intVal = (int)strtol(yytext,&ptr,8);
    // printf("INT:%s\n",yytext);
    // printf("八进制:%d\n",yylval.node->val.intVal);
    return INT;
}

{hexical} {
    yylval.node->type = Node_INT;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    char* ptr;
    yylval.node->val.intVal = (int)strtol(yytext,&ptr,16);
    // printf("INT:%s\n",yytext);
    // printf("十六进制:%d\n",yylval.node->val.intVal);
    return INT;
}

{id} {
    //printf("ID:%s\n",yytext);
    // TODO: add more information about id?
    yylval.node->type = Node_ID;
    yylval.node->numChildren = 0;
    yylval.node->line = yylineno;
    //复制标识符名称，要分配空间
    yylval.node->val.idName = (char*)malloc(sizeof(char)*(yyleng+1));
    memcpy(yylval.node->val.idName,yytext,sizeof(char)*(yyleng+1));
    // printf("id:%s\n",yylval.node->val.idName);
    return ID;
}

. {
    printf("Error type A at line %d:Mysterious character \'%c\'\n",yylineno,*yytext);
}

