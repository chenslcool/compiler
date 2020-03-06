#include<stdio.h>
#include "tree.h"
//已经在syntax.y中define YYDEBUG，要切换debug,只要在这里注释YYDEBUG就行
#define YYDEBUG 1
#include "syntax.tab.h"
void yyrestart ( FILE *input_file  );
int main(int argc,char** argv){
    if(argc > 1){
        FILE* f = fopen(argv[1],"r");
        if(!f){
            perror(argv[1]);
            return 1;
        }
        yyrestart(f);
        #if YYDEBUG
            yydebug = 1;
        #endif
        int err =  yyparse();
        if(err){
            printf("error occur\n");
        }
        else{
            //打印root树
        }
        return 0;
    }
}