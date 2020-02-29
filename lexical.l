%{
    #include<stdio.h>
    #include"syntax.tab.h"
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

digit [0-9]
digits {digit}+
digit8 [0-7]
digit16 [0-9A-Fa-f]
letter [a-zA-Z]
_letter {letter}|(_)
_letterDigit {_letter}|{digit}
whiteSpace [ \t]
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

\n {
    yycolumn = 1;
}

";" {
    printf("SEMI\n");
}

"," {
    printf("COMMA\n");
}

{relop} {
    printf("RELOP:%s\n",yytext);
}

"=" {
    printf("ASSIGNOP\n");
}


"+" {
    // printf("PLUS\n");
    return ADD;
}

"-" {
    // printf("MINUS\n");
    return SUB;
}

"*" {
    // printf("STAR\n");
    return MUL;
}

"/" {
    // printf("DIV\n");
    return DIV;
}

"&&" {
    printf("AND\n");
}

"||" {
    printf("OR\n");
}

"!" {
    printf("NOT\n");
}

"." {
    printf("DOT\n");
}

(int)|(float) {
    printf("TYPE:%s\n",yytext);
}

"(" {
    printf("LP\n");
}

")" {
    printf("RP\n");
}

"[" {
    printf("LB\n");
}

"]" {
    printf("RB\n");
}

"{" {
    printf("LC\n");
}

"}" {
    printf("RC\n");
}

"struct" {
    printf("STRUCT\n");
}

"return" {
    printf("RETURN\n");
}

"if" {
    printf("IF\n");
}

"else" {
    printf("ELSE\n");
}

"while" {
    printf("WHILE\n");
}

{float} {
    // printf("FLOAT :%s\n",yytext);
    yylval.type_float = atof(yytext);
    return FLOAT;
}

{hexical}|{octal}|{decimal} {
    // printf("INT:%s\n",yytext);
    yylval.type_int = atoi(yytext);
    return INT;
}

{id} {
    printf("ID:%s\n",yytext);
}

. {
    printf("Error type A at line %d:Mysterious character \'%s\'\n",yylineno,yytext);
}

