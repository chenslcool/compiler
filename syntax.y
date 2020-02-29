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
%token ADD SUB MUL DIV 

/* 非终结符为了更好的精度选double*/
%type <type_float> Exp Factor Term

%%
Calc : /*empty*/
    | Exp {printf("=%lf\n",$1);}
    ;
Exp : Factor
    | Exp ADD Factor {$$ = $1 + $3;}
    | Exp SUB Factor {$$ = $1 - $3;}
    ;
Factor : Term
    | Factor MUL Term {$$ = $1 * $3;}
    | Factor DIV Term {$$ = $1 / $3;}
    ;
Term : INT {$$ = $1;}
    | FLOAT {$$ = $1;}
    ;

%%

void yyerror(char* msg){
    fprintf(stderr,"error: %s\n",msg);
}