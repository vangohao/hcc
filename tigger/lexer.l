%{
#include"y.tab.h"
#include"analyz.h"
int Tcount = -1;
int tcount = -1;
int Analyz::vcount = -1;
vector<int> Tmap;
vector<int> tmap;
%}
%%
goto   {return GOTO;}
return {return RETURN;}
end    {return END;}
if     {return IF;}
var    {return VAR;}
call   {return CALL;}
param  {return PARAM;}
"\n"   {return '\n';}
[ \t]+   {}
"+"    {yylval.val = '+'; return AOP;}
"-"    {yylval.val = '-'; return AOP;}
"*"    {yylval.val = '*'; return AOP;}
"/"    {yylval.val = '/'; return AOP;}
"%"    {yylval.val = '%'; return AOP;}
"<"    {yylval.val = '<'; return LOP;}
">"    {yylval.val = '>'; return LOP;}
"=="   {yylval.val = 'e'; return LOP;}
"!="   {yylval.val = 'n'; return LOP;} 
":"    {return ':';}
"="    {return '=';}
"["    {return '[';}
"]"    {return ']';}
[0-9]+ {yylval.val = atoi(yytext); return INTEGER;}
"f_"[A-Za-z_][A-Za-z_0-9]*   {yylval.name = yytext; return FUNCTION;}
T[0-9]+                   {int temp = atoi(yytext+1); 
                            if(temp>Tcount){Tcount++;Analyz::vcount++;Tmap.push_back(Analyz::vcount);}
                             yylval.val = Tmap[temp];
                             return VARIABLE;}
t[0-9]+                   {int temp = atoi(yytext+1); 
                            if(temp>tcount){tcount++;Analyz::vcount++;tmap.push_back(Analyz::vcount);}
                             yylval.val = tmap[temp];
                             return VARIABLE;}
p[0-9]+                      {yylval.val = atoi(yytext+1); return PARAMVARIABLE;}
l[0-9]+                      {yylval.val = atoi(yytext+1); return LABEL;}
%%