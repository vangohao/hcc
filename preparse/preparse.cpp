#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 0xffffff
int main()
{
    int ch;
    int status = 0;
    char yytext[MAX];
    int yylength = 0;
    int linenum = 1;
    printf("\n0#");
    while(ch=getchar() != EOF)
    {
        yytext[yylength] = ch;
        yylength ++;
        yytext[yylength] = 0;
        switch(ch)
        {
            case 'f': if(status == 0) status = 1; else {printf("%s",yytext);yylength = 0; yytext[0] = 0;status = -1;} break;
            case 'o': if(status == 1) status = 2; else {printf("%s",yytext);yylength = 0; yytext[0] = 0;status = -1;} break;
            case 'r': if(status == 2) status = 3; else {printf("%s",yytext);yylength = 0; yytext[0] = 0;status = -1;} break;
            case ' ':
            case '\t':
                break;
            case '\n':
                 
                
            case '(':
                if(status == 3) {status = 4;} else {printf()}
        }
    }
    return 0;
}