%locations
%{
    #define YYDEBUG 1
    #include<stdio.h>
    #include "tree.h"
    #include "lex.yy.c"
    extern int SyntaxError;
    void yyerror(char* msg);
    void PrintSyntaxError(char* msg,int line);
%}

/* declared tokens*/
%union {
    struct TreeNode* node;//语法单元的属性值都是树节点
}
%token <node>INT 
%token <node>FLOAT
%token <node>ID /* char* ??*/
%token <node>SEMI <node>COMMA <node>ASSIGNOP <node>RELOP <node>PLUS <node>STAR <node>DIV <node>AND <node>OR <node>DOT <node>NOT <node>TYPE <node>LP <node>RP <node>LB <node>RB <node>LC <node>RC <node>STRUCT <node>RETURN <node>IF <node>ELSE <node>WHILE
%token <node>MINUS /* 减号 */
%token <node>NEGETIVE /* 负号 */
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT NEGETIVE
%left LP RP LB RB DOT
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

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
Program : ExtDefList {$$ = insert(Node_Program,1,($1)); root = ($$);}
    ;
ExtDefList : ExtDef ExtDefList {$$ = insert(Node_ExtDefList,2,($1),($2));}
    | {$$ = insert(Node_ExtDefList,0);}
    ;
ExtDef : Sepcifier ExtDecList SEMI {$$ = insert(Node_ExtDef,3,($1),($2),($3));}
    | Sepcifier SEMI {$$ = insert(Node_ExtDef,2,($1),($2));}
    | Sepcifier FuncDec Compst {$$ = insert(Node_ExtDef,3,($1),($2),($3));}
    | Sepcifier error SEMI{
        //int a = 4;不能赋值
        $$ = insert(Node_ExtDef,0);
        PrintSyntaxError("ExtDef error",$1->line);
    }
    | Sepcifier error{
        $$ = insert(Node_ExtDef,0);
        PrintSyntaxError("ExtDef error",$1->line);
    }
    | error{
        
    }
    ;
ExtDecList : VarDec {$$ = insert(Node_ExtDecList,1,($1));}
    | VarDec COMMA ExtDecList {$$ = insert(Node_ExtDecList,3,($1),($2),($3));}
    ;
Sepcifier : TYPE {$$ = insert(Node_Sepcifier,1,($1));}
    | StructSpecifier {$$ = insert(Node_Sepcifier,1,($1));}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = insert(Node_StructSpecifier,5,($1),($2),($3),($4),($5));}
    | STRUCT Tag {$$ = insert(Node_StructSpecifier,2,($1),($2));}
    | STRUCT error RC SEMI {
        //结构体定义错误
        $$ = insert(Node_StructSpecifier,0);
        PrintSyntaxError("struct define error",$1->line);
    }
    ;
OptTag : ID {$$ = insert(Node_OptTag,1,($1));}
    | {$$ = insert(Node_OptTag,0);}
    ;
Tag : ID {$$ = insert(Node_Tag,1,($1));}
    ;
VarDec : ID {$$ = insert(Node_VarDec,1,($1));}
    | VarDec LB INT RB {$$ = insert(Node_VarDec,4,($1),($2),($3),($4));}
    | VarDec LB error RB{
        //数组定义错误int a[3,4]
        $$ = insert(Node_VarDec,0);
        PrintSyntaxError("array define error",$1->line);
    }
    ;
FuncDec : ID LP VarList RP {$$ = insert(Node_FuncDec,4,($1),($2),($3),($4));}
    | ID LP RP {$$ = insert(Node_FuncDec,3,($1),($2),($3));}
    | ID LP error RP{
        $$ = insert(Node_FuncDec,0);
        PrintSyntaxError("Function header define error",$1->line);
    }
    ;
VarList : ParamDec COMMA VarList {$$ = insert(Node_VarList,3,($1),($2),($3));}
    | ParamDec {$$ = insert(Node_VarList,1,($1));}
    ;
ParamDec : Sepcifier VarDec {$$ = insert(Node_ParamDec,2,($1),($2));}
    ;
Compst : LC DefList StmtList RC {$$ = insert(Node_Compst,4,($1),($2),($3),($4));}
    ;
StmtList : Stmt StmtList {$$ = insert(Node_StmtList,2,($1),($2));}
    | {$$ = insert(Node_StmtList,0);}
    ;
Stmt : Exp SEMI {$$ = insert(Node_Stmt,2,($1),($2));}
    | Exp error SEMI{
        $$ = insert(Node_Stmt,0);
        PrintSyntaxError("Exp statement error",$1->line);
    }
    | Exp error{
        $$ = insert(Node_Stmt,0);
        PrintSyntaxError("Exp statement error",$1->line);
    }
    | Compst {$$ = insert(Node_Stmt,1,($1));}
    | RETURN Exp SEMI {$$ = insert(Node_Stmt,3,($1),($2),($3));}
    | RETURN error SEMI {
        $$ = insert(Node_Stmt,0);
        PrintSyntaxError("Return statement error",$1->line);
    }
    | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE {$$ = insert(Node_Stmt,5,($1),($2),($3),($4),($5));}
    | IF LP Exp RP Stmt ELSE Stmt {$$ = insert(Node_Stmt,7,($1),($2),($3),($4),($5),($6),($7));}
    | IF LP Exp RP error SEMI{
        //if 出错
        $$ = insert(Node_Stmt,0);
        PrintSyntaxError("Missing ';'",$1->line);
    }
    | WHILE LP Exp RP Stmt {$$ = insert(Node_Stmt,5,($1),($2),($3),($4),($5));}
    ;
DefList : Def DefList {$$ = insert(Node_DefList,2,($1),($2));}
    | {$$ = insert(Node_DefList,0);}
    ;
Def : Sepcifier DecList SEMI {$$ = insert(Node_Def,3,($1),($2),($3));}
    | Sepcifier error SEMI{
        $$ = insert(Node_Def,0);
        PrintSyntaxError("Loacl variable error",$1->line);
    }
    ;
DecList : Dec {$$ = insert(Node_DecList,1,($1));}
    | Dec COMMA DecList {$$ = insert(Node_DecList,3,($1),($2),($3));}
    ;
Dec : VarDec {$$ = insert(Node_Dec,1,($1));}
    | VarDec ASSIGNOP Exp {$$ = insert(Node_Dec,3,($1),($2),($3));}
    ;
Exp : Exp ASSIGNOP Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp AND Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp OR Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp RELOP Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp PLUS Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp MINUS Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp STAR Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp DIV Exp {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | LP Exp RP {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | NEGETIVE Exp {$$ = insert(Node_Exp,2,($1),($2));}
    | NOT Exp {$$ = insert(Node_Exp,2,($1),($2));}
    | ID LP Args RP {$$ = insert(Node_Exp,4,($1),($2),($3),($4));}
    | ID LP error SEMI{
        $$ = insert(Node_Exp,0);
        PrintSyntaxError("Function call args error",$1->line);
    }
    | ID LP RP {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | Exp LB Exp RB {$$ = insert(Node_Exp,4,($1),($2),($3),($4));}
    | Exp LB error RB{
        //数组访问错误a[3,4],a[]
        $$ = insert(Node_Exp,0);
        PrintSyntaxError("array access error",$1->line);
    }
    | Exp DOT ID {$$ = insert(Node_Exp,3,($1),($2),($3));}
    | ID {$$ = insert(Node_Exp,1,($1));}
    | INT {$$ = insert(Node_Exp,1,($1));}
    | FLOAT {$$ = insert(Node_Exp,1,($1));}
    | INT error ID{
        //return 表达式错误 23GG这样的
        $$ = insert(Node_Exp,0);
        PrintSyntaxError("Bad Integer",$1->line);
    }
    | FLOAT error ID{
        //return 表达式错误 0.12e这样的
        $$ = insert(Node_Exp,0);
        PrintSyntaxError("Bad Float",$1->line);
    }
    ;
Args : Exp COMMA Args {$$ = insert(Node_Args,3,($1),($2),($3));}
    | Exp {$$ = insert(Node_Args,1,($1));}
    ;
%%

void yyerror(char* msg){
    SyntaxError = 1; 
    // fprintf(stderr,"error: %s\n",msg);
}

void PrintSyntaxError(char* msg,int line){
    printf("Error type B at Line %d: %s.\n",line,msg);
}