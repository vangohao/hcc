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
%token<val> INTEGER AOP LOP VARIABLE LABEL PARAMVARIABLE
%type<expr> Expression ExpressionWithLabel Statements
%type<val> Symbol
%%
Program: 
Program ProgramStatement  '\n'
|
%empty
;
ProgramStatement:
FunctionDecl
|GlobalDeclaration 
;
GlobalDeclaration: 
VAR Symbol                                   {Analyz::Instance.insert($2,4,0);}
| VAR INTEGER Symbol                         {Analyz::Instance.insert($3,4*($2),1);}
;
Declaration: 
VAR Symbol                                   {}
| VAR INTEGER Symbol                         {Analyz::Instance.currentFunc().insert(4*($2));}
;
FunctionDecl:
FUNCTION '[' INTEGER ']' '\n'                  {Analyz::Instance.funcs.push_back(Func($3,$1));
                                                Analyz::Instance.FuncMap[$1] = &(Analyz::Instance.currentFunc());
                                                }
Statements                                     {}                        
;
Statements:
Statements ExpressionWithLabel '\n'            {if($1->type != Goto) $1->nexts.push_back($2); 
                                                $$=$2;} 
/*
|Statements LABEL ':' '\n'                     {$$=new Expression(Label,{},{},{$2});
                                                $1->nexts.push_back($$);
                                                Analyz::Instance.labelTable[$2] = $$;}
*/
|Statements Declaration '\n'                       {$$ = $1;}    
|%empty                                        {$$=new Expression(Begin,{},{},{});}
;
ExpressionWithLabel:
LABEL ':' '\n' ExpressionWithLabel             {$$=$4; Analyz::Instance.labelTable[$1] = $4;}
|Expression                                    {$$=$1;}
;
Expression:
Symbol '=' Symbol AOP Symbol             {  if($3!=$5)
                                                $$=new Expression(ArithRR,{$1},{$3,$5},{});
                                            else
                                                $$=new Expression(ArithRRSame,{$1},{$3},{});
                                                }
| Symbol '=' Symbol AOP INTEGER            {$$=new Expression(ArithRI,{$1},{$3},{$5});} 
//AOP allow + * only
/*
| Symbol '=' INTEGER AOP Symbol           {
                                        if($4=='+') $$=new Expression(ArithRR,{$1},{$3},{$5});
                                        }
*/
| Symbol '=' INTEGER AOP INTEGER            {$$=new Expression(MoveRI,{$1},{},{calcarith($3,$4,$5)});}
| Symbol '=' AOP Symbol                   {$$=new Expression(Negative,{$1},{$4},{});}
| Symbol '=' AOP INTEGER                    {$$=new Expression(MoveRI,{$1},{},{calcarith(0,$3,$4)});}
| Symbol '=' Symbol                       {
    if($1 != $3)
    $$=new Expression(MoveRR,{$1},{$3},{});
    else 
    $$=new Expression(Empty,{},{},{});
    }
| Symbol '=' INTEGER                        {$$=new Expression(MoveRI,{$1},{},{$3});}
/*
| Symbol '[' Symbol ']' '=' Symbol
| Symbol '[' Symbol ']' '=' INTEGER
*/
| Symbol '[' INTEGER ']' '=' Symbol       {$$=new Expression(ArrayWrite,{},{$1,$6},{$3});}
//| Symbol '[' INTEGER ']' '=' INTEGER        {$$=new Expression(ArrayWriteR,{$1},{$6},{});}
//| Symbol '[' '*' ']' '=' Symbol           {$$=new Expression(ArrayWriteR,{$1},{$6},{0});}
//| Symbol '[' '*' ']' '=' INTEGER            {$$=new Expression(ArrayWriteI,{$1},{},{0,$6});}
//| Symbol '=' Symbol '[' '*' ']'             {$$=new Expression(ArrayRead,{$1},{$3},{0});}
| Symbol '=' Symbol '[' INTEGER ']'         {$$=new Expression(ArrayRead,{$1},{$3},{$5});}
| IF Symbol LOP Symbol GOTO LABEL           {$$=new Expression(IfRR,{},{$2,$4},{$3,$6});}
| IF Symbol LOP INTEGER GOTO LABEL            {$$=new Expression(IfRI,{},{$2},{$3,$6,$4});}
| IF INTEGER LOP Symbol GOTO LABEL            {$$=new Expression(IfIR,{},{$4},{$3,$6,$2});}
| IF INTEGER LOP INTEGER GOTO LABEL             {if(calclogic($2,$3,$4)) $$=new Expression(Goto,{},{},{$6});}
| GOTO LABEL                                    {$$=new Expression(Goto,{},{},{$2});}
| PARAM Symbol                                {$$=new Expression(ParamR,{},{$2},{});}
| PARAM INTEGER                                 {$$=new Expression(ParamI,{},{},{$2});}
| Symbol '=' CALL FUNCTION                    {$$=new Expression(Call,{$1},{},{},$4);}
| RETURN Symbol                               {$$=new Expression(ReturnR,{},{$2},{});}
| RETURN INTEGER                                {$$=new Expression(ReturnI,{},{},{$2});}
| END FUNCTION                                  {$$=new Expression(Empty,{},{},{});}
;
Symbol:
VARIABLE                                        {$$ = $1;}
|PARAMVARIABLE                                  {$$ = Analyz::Instance.currentFunc().paramTable[$1];}

%%