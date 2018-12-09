%{
#include"y.tab.h"
%}
%%
malloc {return MALLOC;}
store  {return STORE;}
load   {return LOAD;}
loadaddr   {return LOADADDR;}
goto   {return GOTO;}
return {return RETURN;}
end    {return END;}
if     {return IF;}
call   {return CALL;}
[ \t\n]+   {}
"+"|"-"|"*"|"/"|"%"    {return yytext[0];}
"<"|">"    { return yytext[0];}
"=="   {return 'e';}
"!="   {return 'n';} 
":"    {return ':';}
"="    {return '=';}
"["    {return '[';}
"]"    {return ']';}
[0-9]+ {yylval.val = atoi(yytext); return INTEGER;}
"f_"[A-Za-z_][A-Za-z_0-9]*   {yylval.name = strdup(yytext); return FUNCTION;}
v[0-9]+                      {yylval.name = strdup(yytext); return GLOBAL;}
l[0-9]+                      {yylval.name = strdup(yytext); return LABEL;}
x0|s[0-9]|s10|s11|a[0-7]|t[0-6]   {yylval.name = strdup(yytext); return REG;}
%%
