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
    // printf("SEMI\n");
    return SEMI;
}

"," {
    // printf("COMMA\n");
    return COMMA;
}

{relop} {
    // printf("RELOP:%s\n",yytext);
    return RELOP;
}

"=" {
    // printf("ASSIGNOP\n");
    return ASSIGNOP;
}


"+" {
    // printf("PLUS\n");
    return PLUS;
}

"-" {
    // printf("MINUS\n");
    return MINUS;
}

"*" {
    // printf("STAR\n");
    return STAR;
}

"/" {
    // printf("DIV\n");
    return DIV;
}

"&&" {
    // printf("AND\n");
    return AND;
}

"||" {
    // printf("OR\n");
    return OR;
}

"!" {
    // printf("NOT\n");
    return NOT;
}

"." {
    // printf("DOT\n");
    return DOT;
}

(int)|(float) {
    // printf("TYPE:%s\n",yytext);
    return TYPE;
}

"(" {
    // printf("LP\n");
    return LP;
}

")" {
    // printf("RP\n");
    return RP;
}

"[" {
    // printf("LB\n");
    return LB;
}

"]" {
    // printf("RB\n");
    return RB;
}

"{" {
    // printf("LC\n");
    return LC;
}

"}" {
    // printf("RC\n");
    return RC;
}

"struct" {
    // printf("STRUCT\n");
    return STRUCT;
}

"return" {
    // printf("RETURN\n");
    return RETURN;
}

"if" {
    // printf("IF\n");
    return IF;
}

"else" {
    // printf("ELSE\n");
    return ELSE;
}

"while" {
    // printf("WHILE\n");
    return WHILE;
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
    // printf("ID:%s\n",yytext);
    // TODO: add more information about id?
    return ID;
}

. {
    printf("Error type A at line %d:Mysterious character \'%s\'\n",yylineno,yytext);
}

