%locations
%{
    #include<stdio.h>
    #include "tree.h"
    #include "lex.yy.c"
    void yyerror(char* msg);
%}

/* declared tokens*/
%union {
    struct TreeNode node;//语法单元的属性值都是树节点
}
%token <node>INT 
%token <node>FLOAT
%token <node>ID /* char* ??*/
%token <node>SEMI <node>COMMA <node>ASSIGNOP <node>RELOP <node>PLUS <node>STAR <node>DIV <node>AND <node>OR <node>DOT <node>NOT <node>TYPE <node>LP <node>RP <node>LB <node>RB <node>LC <node>RC <node>STRUCT <node>RETURN <node>IF <node>ELSE <node>WHILE
%token <node>MINUS 

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%type <node> Program
%type <node> ExtDefList
%type <node> ExtDef
%type <node> ExtDecList
%type <node> Sepcifier 
%type <node> StructSpecifier
%type <node> OptTag
%type <node> Tag
%type <node> VarDec
%type <node> FuncDec
%type <node> VarList
%type <node> ParamDec
%type <node> Compst
%type <node> StmtList
%type <node> Stmt
%type <node> DefList
%type <node> Def
%type <node> DecList
%type <node> Dec
%type <node> Exp
%type <node> Args
%%
Program : ExtDefList {insert(&($$),1,&($1));}
    ;
ExtDefList : ExtDef ExtDefList {insert(&($$),2,&($1),&($2));}
    | {insert(&($$),0);}
    ;
ExtDef : Sepcifier ExtDecList SEMI {insert(&($$),3,&($1),&($2),&($3));}
    | Sepcifier SEMI {insert(&($$),2,&($1),&($2));}
    | Sepcifier FuncDec Compst {insert(&($$),3,&($1),&($2),&($3));}
    ;
ExtDecList : VarDec {insert(&($$),1,&($1));}
    | VarDec COMMA ExtDecList {insert(&($$),3,&($1),&($2),&($3));}
    ;
Sepcifier : TYPE {insert(&($$),1,&($1));}
    | StructSpecifier {insert(&($$),1,&($1));}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {insert(&($$),5,&($1),&($2),&($3),&($4),&($5));}
    | STRUCT Tag {insert(&($$),2,&($1),&($2));}
    ;
OptTag : ID {insert(&($$),1,&($1));}
    | {insert(&($$),0);}
    ;
Tag : ID {insert(&($$),1,&($1));}
    ;
VarDec : ID {insert(&($$),1,&($1));}
    | VarDec LB INT RB {insert(&($$),4,&($1),&($2),&($3),&($4));}
    ;
FuncDec : ID LP VarList RP {insert(&($$),4,&($1),&($2),&($3),&($4));}
    | ID LP RP {insert(&($$),3,&($1),&($2),&($3));}
    ;
VarList : ParamDec COMMA VarList {insert(&($$),3,&($1),&($2),&($3));}
    | ParamDec {insert(&($$),1,&($1));}
    ;
ParamDec : Sepcifier VarDec {insert(&($$),2,&($1),&($2));}
    ;
Compst : LC DefList StmtList RC {insert(&($$),4,&($1),&($2),&($3),&($4));}
    ;
StmtList : Stmt StmtList {insert(&($$),2,&($1),&($2));}
    | {insert(&($$),0);}
    ;
Stmt : Exp SEMI {insert(&($$),2,&($1),&($2));}
    | Compst {insert(&($$),1,&($1));}
    | RETURN Exp SEMI {insert(&($$),3,&($1),&($2),&($3));}
    | IF LP Exp RP Stmt {insert(&($$),5,&($1),&($2),&($3),&($4),&($5));}
    | IF LP Exp RP Stmt ELSE Stmt {insert(&($$),7,&($1),&($2),&($3),&($4),&($5),&($6),&($7));}
    | WHILE LP Exp RP Stmt {insert(&($$),5,&($1),&($2),&($3),&($4),&($5));}
    ;
DefList : Def DefList {insert(&($$),2,&($1),&($2));}
    | {insert(&($$),0);}
    ;
Def : Sepcifier DecList SEMI {insert(&($$),3,&($1),&($2),&($3));}
    ;
DecList : Dec {insert(&($$),1,&($1));}
    | Dec COMMA DecList {insert(&($$),3,&($1),&($2),&($3));}
    ;
Dec : VarDec {insert(&($$),1,&($1));}
    | VarDec ASSIGNOP Exp {insert(&($$),3,&($1),&($2),&($3));}
    ;
Exp : Exp ASSIGNOP Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp AND Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp OR Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp RELOP Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp PLUS Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp MINUS Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp STAR Exp {insert(&($$),3,&($1),&($2),&($3));}
    | Exp DIV Exp {insert(&($$),3,&($1),&($2),&($3));}
    | LP Exp RP {insert(&($$),3,&($1),&($2),&($3));}
    | MINUS Exp {insert(&($$),2,&($1),&($2));}
    | NOT Exp {insert(&($$),2,&($1),&($2));}
    | ID LP Args RP {insert(&($$),4,&($1),&($2),&($3),&($4));}
    | ID LP RP {insert(&($$),3,&($1),&($2),&($3));}
    | Exp LB Exp RB {insert(&($$),4,&($1),&($2),&($3),&($4));}
    | Exp DOT ID {insert(&($$),3,&($1),&($2),&($3));}
    | ID {insert(&($$),1,&($1));}
    | INT {insert(&($$),1,&($1));}
    | FLOAT {insert(&($$),1,&($1));}
    ;
Args : Exp COMMA Args {insert(&($$),3,&($1),&($2),&($3));}
    | Exp {insert(&($$),1,&($1));}
    ;
%%

void yyerror(char* msg){
    fprintf(stderr,"error: %s\n",msg);
}