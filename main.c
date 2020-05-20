#include<stdio.h>
#include "tree.h"
//已经在syntax.y中define YYDEBUG，要切换debug,只要在这里注释YYDEBUG就行
// #define YYDEBUG 1
#include "syntax.tab.h"
#include "semantic.h"
int SyntaxError = 0;//bison检查到语义错误，在含error的产生式的语义动作中会设置它，给main()查看
int containsStruct = 0;
void yyrestart ( FILE *input_file  );
int main(int argc,char** argv){
    if(argc > 2){
        FILE* fin = fopen(argv[1],"r");
        if(!fin){
            perror(argv[1]);
            return 1; 
        }
        yyrestart(fin);
        #if YYDEBUG
            yydebug = 1;
        #endif
        yyparse();
        if(SyntaxError == 0){
            // preTraverse(root,0);
            if(containsStruct == 1){
                fprintf(stderr, "Cannot translate: Code contains variables or parameters of structure type.\n");
            }
            else{
                FILE* fout_ir = fopen(argv[2],"w");
                FILE* fout_s = fopen(argv[3],"w");
                initReadAndWrite();
                handleProgram(root);
                printInterCodeList(fout_ir);
                fclose(fout_ir);
                translateToMachineCode(fout_s);
                fclose(fout_s);
            }
            // fprintf(stderr,"HashSize = %d,numHashSearch = %d\n",TABLE_SIZE,numHashSearch);
        }
        fclose(fin);
    }
    else{
        printf("arguement not enough!\n");
    }
    return 0;
}