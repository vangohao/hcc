%{
#include "y.tab.h"
%}

%%
"/*"([^/*]*|[^/]*[^*]+"/")*"*/"         {}
==    {linenum = yylineno; return EQUAL;}
!=    {linenum = yylineno; return NOTEQUAL;}
&&    {linenum = yylineno; return LAND;}
"||"  {linenum = yylineno; return LOR;}
[0-9]+  {yylval.number = atoi(yytext); linenum = yylineno; return INTEGER;}
"+" {linenum = yylineno; return '+';}
"-" {linenum = yylineno; return '-';}
"*" {linenum = yylineno; return '*';}
"/" {linenum = yylineno; return '/';}
"%" {linenum = yylineno; return '%';}
"<" {linenum = yylineno; return '<';}
">" {linenum = yylineno; return '>';}
"!" {linenum = yylineno; return '!';}
"[" {linenum = yylineno; return '[';}
"]" {linenum = yylineno; return ']';}
"=" {linenum = yylineno; return '=';}
"," {linenum = yylineno; return ',';}
";" {linenum = yylineno; return ';';}
"(" {linenum = yylineno; return '(';}
")" {linenum = yylineno; return ')';}
"{" {linenum = yylineno; return '{';}
"}" {linenum = yylineno; return '}';}
int     {linenum = yylineno; return T_INT;}
main   {linenum = yylineno; return MAIN;}
if      {linenum = yylineno; return IF;}
else    {linenum = yylineno; return ELSE;}
while   {linenum = yylineno; return WHILE;}
return  {linenum = yylineno; return RETURN;}
[_A-Za-z][_A-Za-z0-9]* {yylval.name = strdup(yytext); linenum = yylineno; return IDENTIFIER;}
[ \t\n]+ {}
%%
int yywrap()
{
    linenum = yylineno; return 1;
}