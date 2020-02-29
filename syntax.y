%locations
%{
    #include<stdio.h>
    #include "lex.yy.c"
    void yyerror(char* msg);
%}

/* declared tokens*/
%union {
    int type_int;
    float type_float;
    double type_double; 
}
%token <type_int>INT 
%token <type_float>FLOAT
%token ID /* char* ??*/
%token SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND OR DOT NOT TYPE LP RP LB RB LC RC STRUCT RETURN IF ELSE WHILE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%
Program : ExtDefList
    ;
ExtDefList : ExtDef ExtDefList
    |
    ;
ExtDef : Sepcifier ExtDecList SEMI
    | Sepcifier SEMI
    | Sepcifier FuncDec Compst
    ;
ExtDecList : VarDec
    | VarDec COMMA ExtDecList
    ;
Sepcifier : TYPE
    | StructSpecifier
    ;
StructSpecifier : STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag : ID
    |
    ;
Tag : ID
    ;
VarDec : ID
    | VarDec LB INT RB
    ;
FuncDec : ID LP VarList RP
    | ID LP RP
    ;
VarList : ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec : Sepcifier VarDec
    ;
Compst : LC DefList StmtList RC
    ;
StmtList : Stmt StmtList
    |
    ;
Stmt : Exp SEMI
    | Compst
    | RETURN Exp Stmt
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;
DefList : Def DefList
    |
    ;
Def : Sepcifier DecList SEMI
    ;
DecList : Dec
    | Dec COMMA DecList
    ;
Dec : VarDec
    | VarDec ASSIGNOP Exp
    ;
Exp : Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;
Args : Exp COMMA Args
    | Exp
    ;
%%

void yyerror(char* msg){
    fprintf(stderr,"error: %s\n",msg);
}