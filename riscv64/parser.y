%code requires{
#include<iostream>
using std::cerr;using std::endl;
int yylex();
}

%union{
    int val;
    char * name;
}

%code{
int yyerror(const char * s);
}

%token<name> REG FUNCTION LABEL
%token<val> GLOBAL INTEGER
%token MALLOC IF GOTO CALL STORE LOAD LOADADDR RETURN END

%%
Goal: Goal FunctionDecl 
| Goal GlobalVarDecl
| %empty
;
GlobalVarDecl : GLOBAL '=' INTEGER
| GLOBAL '=' MALLOC INTEGER
;
FunctionDecl : FUNCTION '[' INTEGER ']' '[' INTEGER ']' Expressions END FUNCTION
;
Expressions : Expressions Expression
| Expression
;
Expression : REG '=' REG '+' REG
| REG '=' REG '-' REG
| REG '=' REG '*' REG
| REG '=' REG '/' REG
| REG '=' REG '%' REG
| REG '=' REG '+' INTEGER
| REG '=' REG '*' INTEGER
| REG '=' '-' REG
| REG '=' INTEGER
| REG '[' INTEGER ']' '=' REG
| REG '=' REG '[' INTEGER ']'
| IF REG '<' REG GOTO LABEL
| IF REG '>' REG GOTO LABEL
| IF REG 'e' REG GOTO LABEL
| IF REG 'n' REG GOTO LABEL
| GOTO LABEL
| LABEL ':'
| CALL FUNCTION
| STORE REG INTEGER
| LOAD INTEGER REG
| LOAD GLOBAL REG
| LOADADDR INTEGER REG
| LOADADDR GLOBAL REG
| RETURN
;
%%
int yyerror(const char * s)
{
    cerr<<s<<endl;
    return 1;
}
