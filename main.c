#include<stdio.h>
#include "tree.h"
//已经在syntax.y中define YYDEBUG，要切换debug,只要在这里注释YYDEBUG就行
// #define YYDEBUG 1
#include "syntax.tab.h"
#include "semantic.h"
int SyntaxError = 0;//bison检查到语义错误，在含error的产生式的语义动作中会设置它，给main()查看
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
        yyparse();
        if(SyntaxError == 0){
            // preTraverse(root,0);
            handleProgram(root);
            fprintf(stderr,"HashSize = %d,numHashSearch = %d\n",TABLE_SIZE,numHashSearch);
        }
        return 0;
    }
}