#include"tigger.h"
int yyerror(string s)
{
    cerr<<s<<endl;
}
extern "C"
{
int yywrap()
{
    return 1;
}
}
int calcarith(int a,int op,int b)
{
    if(op=='+') return a+b;
    if(op=='*') return a*b;
    if(op=='-') return a-b;
    if(op=='/') return a/b;
    if(op=='%') return a%b;
}
int calclogic(int a,int op,int b)
{
    if(op=='<') return a<b;
    if(op=='>') return a>b;
    if(op=='e') return a==b;
    if(op=='n') return a!=b;
}