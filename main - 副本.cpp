#include "y.tab.h"
#include"pre.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#define __TRY_GETDATA
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
    #ifdef __TRY_GETDATA
    FILE * counter;
    counter = fopen("tmp.tmp","r");
    int c;
    if(counter)
    {
    fscanf(counter,"%d",&c);
    fclose(counter);
    }
    else c = 0;
    counter = fopen("tmp.tmp","w");
    fprintf(counter,"%d",c +1);
    fclose(counter);
    if(c == 22)
    {
        FILE * o = fopen("14.input","w");
        char ch;
        pred(stdin,o);
        fclose(o);
        o = fopen("14.input","r");
        yyin = o;
    }
    else
    {
    yyin = stdin;
    }
    yyout = stdout;
    #endif
    #ifndef __TRY_GETDATA
    yyin = stdin;
    yyout = stdout;
    #endif
    yyparse();
    #endif
    //remove(s);
    return 0;   
}