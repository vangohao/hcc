%code requires{
    #include"tigger.h"
    #include"analyz.h"
    struct foryystype
    {
        string name;
        int val;
        Expression* expr;
    };
    typedef foryystype YYSTYPE;
}

%token  VAR END IF RETURN GOTO CALL PARAM
%token<name> FUNCTION
%token<val> INTEGER AOP LOP VARIABLE LABEL
%type<expr> Expression
%%
Program: 
Program ProgramStatement '\n'
|
%empty
;
ProgramStatement:
FunctionDecl
|GlobalDeclaration
;
GlobalDeclaration: 
VAR VARIABLE                                   {Analyz::Instance.insert();}
| VAR INTEGER VARIABLE                         {Analyz::Instance.insert(4*($2));}
;
Declaration: 
VAR VARIABLE                                   {}
| VAR INTEGER VARIABLE                         {}
;
FunctionDecl:
FUNCTION '[' INTEGER ']' '\n'                  {Analyz::Instance.funcs.push_back(Func($3,$1));}
Statements                                      
END FUNCTION                                   
;
Statements:
Statements Statement '\n'                       
|%empty
;
Statement:
Expression
|Declaration
;
Expression:
VARIABLE '=' VARIABLE AOP VARIABLE             {$$=new Expression(ArithRR,{$1},{$3,$5},{});}
/*
| VARIABLE '=' VARIABLE AOP INTEGER            {$$=new Expression(ArithRR,{$1},{$3},{$5});}
| VARIABLE '=' INTEGER AOP VARIABLE           {
                                        if($4=='+') $$=new Expression(ArithRR,{$1},{$3},{$5});
                                        else
                                        {
                                            
                                        }
                                        }
*/
| VARIABLE '=' INTEGER AOP INTEGER            {$$=new Expression(MoveRI,{$1},{},{calcarith($3,$4,$5)});}
| VARIABLE '=' AOP VARIABLE                   {$$=new Expression(Negative,{$1},{$4},{});}
| VARIABLE '=' AOP INTEGER                    {$$=new Expression(MoveRI,{$1},{},{calcarith(0,$3,$4)});}
| VARIABLE '=' VARIABLE                       {$$=new Expression(MoveRR,{$1},{$3},{});}
| VARIABLE '=' INTEGER                        {$$=new Expression(MoveRI,{$1},{},{$3});}
/*
| VARIABLE '[' VARIABLE ']' '=' VARIABLE
| VARIABLE '[' VARIABLE ']' '=' INTEGER
*/
| VARIABLE '[' INTEGER ']' '=' VARIABLE       {$$=new Expression(ArrayWrite,{$1},{$6},{$3});}
//| VARIABLE '[' INTEGER ']' '=' INTEGER        {$$=new Expression(ArrayWriteR,{$1},{$6},{});}
//| VARIABLE '[' '*' ']' '=' VARIABLE           {$$=new Expression(ArrayWriteR,{$1},{$6},{0});}
//| VARIABLE '[' '*' ']' '=' INTEGER            {$$=new Expression(ArrayWriteI,{$1},{},{0,$6});}
//| VARIABLE '=' VARIABLE '[' '*' ']'             {$$=new Expression(ArrayRead,{$1},{$3},{0});}
| VARIABLE '=' VARIABLE '[' INTEGER ']'         {$$=new Expression(ArrayRead,{$1},{$3},{$5});}
| IF VARIABLE LOP VARIABLE GOTO LABEL           {$$=new Expression(IfRR,{},{$2,$4},{$3,$6});}
| IF VARIABLE LOP INTEGER GOTO LABEL            {$$=new Expression(IfRI,{},{$2},{$3,$6,$4});}
| IF INTEGER LOP VARIABLE GOTO LABEL            {$$=new Expression(IfIR,{},{$4},{$3,$6,$2});}
| IF INTEGER LOP INTEGER GOTO LABEL             {/* ?? */}
| GOTO LABEL                                    {$$=new Expression(Goto,{},{},{$2});}
| LABEL ':'                                     {/* ?? */}
| PARAM VARIABLE                                {$$=new Expression(ParamR,{},{$2},{});}
| PARAM INTEGER                                 {$$=new Expression(ParamI,{},{},{$2});}
| VARIABLE '=' CALL FUNCTION                    {$$=new Expression(Call,{$1},{},{},$4);}
| RETURN VARIABLE                               {$$=new Expression(ReturnR,{},{$2},{});}
| RETURN INTEGER                                {$$=new Expression(ReturnI,{},{},{$2});}
;


%%