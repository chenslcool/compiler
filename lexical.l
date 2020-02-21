%{
    int is_letterDigit(char c);
%}
digit [0-9]
digits {digit}+
digit8 [0-7]
digit16 [0-9A-Fa-f]
letter [a-zA-Z]
_letter {letter}|(_)
_letterDigit {_letter}|{digit}
whiteSpace [ \n\t]
relop ("<")|("<=")|(">")|(">=")|("==")|("!=")
decimal ([+-]?)(0|([1-9]({digit}*)))
octal ([+-]?)0({digit8}+)
hexical ([+-]?)(0[Xx])({digit16}+)
float ([+-]?)({digits})(\.)({digits})
id {_letter}(({_letter}|{digit})*)
%option yylineno
%%
{whiteSpace}+ {
    
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
    printf("PLUS\n");
}

"-" {
    printf("MINUS\n");
}

"*" {
    printf("STAR\n");
}

"/" {
    printf("DIV\n");
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

{hexical} {
    //read a hexical,but may have illgal char follow
    //You can record the INT data from yytext first and then find if error follow
    //atoi()
    char c = input();//[0-9a-fA-F] if impossible
    if((c > 'f' && c <= 'z')||(c > 'F' && c <= 'Z')||(c == '_')){
        //cheated
        c = input();
        while(is_letterDigit(c)){
            c = input();
        }
        unput(c);
        printf("Error type A at line %d:invalid hexical,bad char(s) after \" %s \" \n",yylineno,yytext);
    }
    else{
        printf("INT(hexical):%s\n",yytext);
        unput(c);
    }
}

{octal} {
    //read a octal,but may have illgal char follow
    char c = input();//[0-7] if impossible
    if((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')||(c == '_')||(c == '8')||(c == '9')){
        //cheated
        c = input();
        while(is_letterDigit(c)){
            c = input();
        }
        unput(c);
        printf("Error type A at line %d:invalid octal,bad char(s) after \" %s \"\n",yylineno,yytext);
    }
    else{
        printf("INT(octal):%s\n",yytext);
        unput(c);
    }
}

{decimal} {
    //read a decimal,but may have illgal char follow
    char c = input();//[0-9] is impossible
    //要考虑0[8-9]的情况，这用八进制不能匹配，被匹配成十进制的0
    if((((yyleng == 1 && yytext[0] == '0')
    ||(yyleng == 2 && yytext[1] == '0' && (yytext[0] == '+' || yytext[0] == '-')))&& (c >= '8'&&c <= '9'))
    ||((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')||(c == '_'))){
        //cheated
        c = input();
        while(is_letterDigit(c)){
            c = input();
        }
        unput(c);
        printf("Error type A at line %d:invalid decimal,bad char(s) after \" %s \"\n",yylineno,yytext);
    }
    else{
        printf("INT(decimal):%s\n",yytext);
        unput(c);
    }
}
{float} {
    //read a float,but may have illgal char follow
    char c = input();//[0-9] is impossible
    if((c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z')||(c == '_')){
        //cheated
        c = input();
        while(is_letterDigit(c)){
            c = input();
        }
        unput(c);
        printf("Error type A at line %d:bad char(s) after \" %s \"\n",yylineno,yytext);
    }
    else{
        printf("FLOAT :%s\n",yytext);
        unput(c);
    }
}

{id} {
    printf("ID:%s\n",yytext);
}

. {
    printf("Error type A at line %d:Mysterious char \'%s\'\n",yylineno,yytext);
}

%%
int main(int argc,char** argv){
    if(argc > 1){
        if(!(yyin = fopen(argv[1],"r"))){
            perror(argv[1]);
            return 1;
        }
        yylex();
        // printf("%8d%8d%8d,ret = %d\n",lines,words,chars,ret);
        return 0;
    }
}
int is_letterDigit(char c){
    if ((c == '_') || (c >= 'a'&& c <= 'z') || (c >= 'a'&& c <= 'z') || (c >= '0'&& c <= '9')){
        return 1;
    }
    else
        return 0;
}
