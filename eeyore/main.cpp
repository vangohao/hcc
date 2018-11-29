#include "y.tab.h"
#include"pre.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
extern FILE* yyin, *yyout;
int main(int argc, char * args[])
{
    #ifdef __LOCAL_HAO
    FILE* input,* preoutput,* output;
    char * s;
    if(argc <= 1)
    {  
        input = stdin;
        preoutput = fopen(".tmp.pre","w");
        output = stdout;
    }
    else if(argc == 2)
    {
        input = fopen(args[1],"r");
        s = (char *) malloc(strlen(args[1] + 10));
        strcpy(s,args[1]);
        strcat(s,".tmp.pre");
        preoutput = fopen(s,"w");
        output = stdout;
    }
    else if(argc > 2)
    {
        input = fopen(args[1],"r");
        s = (char *) malloc(strlen(args[1] + 10));
        strcpy(s,args[1]);
        strcat(s,".tmp.pre");
        preoutput = fopen(s,"w");
        output = fopen(args[2],"w");
    }
    pred(input,preoutput);
    fclose(preoutput);
    yyin = fopen(s,"r");
    yyout = output;
    yyparse();
    fclose(yyin);
    fclose(yyout);
    #endif
    #ifndef __LOCAL_HAO
    yyin = stdin;
    yyout = stdout;
    yyparse();
    #endif
    //remove(s);
    return 0;   
}