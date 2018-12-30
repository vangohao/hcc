%{
#include "y.tab.h"
%}

%%
"//"[^\n]*\n  							{}
"/*"([^*]|("*"[^/]))*"*/"         {}
==    {return EQUAL;}
!=    {return NOTEQUAL;}
&&    {return LAND;}
"||"  {return LOR;}
[0-9]+  {yylval.number = atoi(yytext); return INTEGER;}
"+" {return '+';}
"-" {return '-';}
"*" {return '*';}
"/" {return '/';}
"%" {return '%';}
"<" {return '<';}
">" {return '>';}
"!" {return '!';}
"[" {return '[';}
"]" {return ']';}
"=" {return '=';}
"," {return ',';}
";" {return ';';}
"(" {return '(';}
")" {return ')';}
"{" {return '{';}
"}" {return '}';}
int     {return T_INT;}
main   {return MAIN;}
if      {return IF;}
else    {return ELSE;}
while   {return WHILE;}
return  {return RETURN;}
[_A-Za-z][_A-Za-z0-9]* {yylval.name = strdup(yytext); return IDENTIFIER;}
[ \t\r\n]+ {}
.   {return IllegalCharacter;}
%%
int yywrap()
{
    return 1;
}