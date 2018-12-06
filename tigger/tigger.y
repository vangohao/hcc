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
%type<expr> Expression Statements
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
| VAR INTEGER Symbol                         {Analyz::Instance.insert($3,$2,1);}
;
Declaration: 
VAR Symbol                                   {}
| VAR INTEGER Symbol                         {Analyz::Instance.currentFunc().insert($2,$3);}
;
FunctionDecl:
FUNCTION '[' INTEGER ']' '\n'                  {Analyz::Instance.funcs.push_back(Func($3,$1));
                                                Analyz::Instance.FuncMap[$1] = &(Analyz::Instance.currentFunc());
                                                }
Statements                                     {}                        
END FUNCTION                                  { new Expression(Empty,{},{},{});}
;
Statements:
Statements Expression '\n'            {
                                                $$=$2;} 
/*
|Statements LABEL ':' '\n'                     {$$=new Expression(Label,{},{},{$2});
                                                $1->nexts.push_back($$);
                                                Analyz::Instance.labelTable[$2] = $$;}
*/
|Statements Declaration '\n'                       {$$ = $1;}    
|%empty                                        {$$=new Expression(Begin,{},{},{});}
;
Expression:
Symbol '=' Symbol AOP Symbol             {  if($3!=$5)
                                                $$=new Expression(ArithRR,{$1},{$3,$5},{$4});
                                            else
                                                $$=new Expression(ArithRRSame,{$1},{$3},{$4});
                                                }
| Symbol '=' Symbol AOP INTEGER            {if($4=='+' || $4=='*')
                                            $$=new Expression(ArithRI,{$1},{$3},{$5,$4});
                                            else
                                            {
                                                int tmp1 = ++Analyz::vcount;
                                                $$ = new Expression(MoveRI,{tmp1},{},{$5});
                                                auto e = new Expression(ArithRR,{$1},{$3,tmp1},{$4});
                                            }
} 
//AOP allow + * only

| Symbol '=' INTEGER AOP Symbol           {
                                    
                                                int tmp1 = ++Analyz::vcount;
                                                $$ = new Expression(MoveRI,{tmp1},{},{$3});
                                                auto e = new Expression(ArithRR,{$1},{tmp1,$5},{$4});
                                        }

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
| Symbol '[' Symbol ']' '=' Symbol          {
                                            int tmp1 = ++Analyz::vcount;
                                            auto e = new Expression(ArithRR,{tmp1},{$1,$3},{'+'});
                                            $$ = new Expression(ArrayWrite,{},{tmp1,$6},{0});
}
/*
| Symbol '[' Symbol ']' '=' INTEGER
*/
| Symbol '[' INTEGER ']' '=' Symbol         {$$=new Expression(ArrayWrite,{},{$1,$6},{$3});}
| Symbol '[' INTEGER ']' '=' INTEGER        {int tmp1 = ++Analyz::vcount;
                                            auto e = new Expression(MoveRI,{tmp1},{},{$6});
                                            $$=new Expression(ArrayWrite,{},{$1,tmp1},{$3});}
| Symbol '=' Symbol '[' INTEGER ']'         {$$=new Expression(ArrayRead,{$1},{$3},{$5});}
| Symbol '=' Symbol '[' Symbol ']'          {
                                            int tmp1 = ++Analyz::vcount;
                                            auto e = new Expression(ArithRR,{tmp1},{$3,$5},{'+'});
                                            $$ = new Expression(ArrayRead,{$1},{tmp1},{0});
}
| IF Symbol LOP Symbol GOTO LABEL           {$$=new Expression(IfRR,{},{$2,$4},{$3,$6});}
| IF Symbol LOP INTEGER GOTO LABEL            {if($4==0)
                                                $$=new Expression(IfRI,{},{$2},{$3,$6,$4});
                                                else
                                                {
                                                    int tmp1 = ++Analyz::vcount;
                                                    auto e = new Expression(MoveRI,{tmp1},{},{$4});
                                                    $$ = new Expression(IfRR,{},{$2,tmp1},{$3,$6});
                                                }

}
| IF INTEGER LOP Symbol GOTO LABEL            {if($2 ==0)
                                                $$=new Expression(IfIR,{},{$4},{$3,$6,$2});
                                                else
                                                {
                                                    int tmp1 = ++Analyz::vcount;
                                                    auto e = new Expression(MoveRI,{tmp1},{},{$2});
                                                    $$ = new Expression(IfRR,{},{tmp1,$4},{$3,$6});
                                                }
}
| IF INTEGER LOP INTEGER GOTO LABEL             {if(calclogic($2,$3,$4)) $$=new Expression(Goto,{},{},{$6});}
| GOTO LABEL                                    {$$=new Expression(Goto,{},{},{$2});}
| PARAM Symbol                                {Analyz::Instance.currentFunc().CallParam($2,1);}
| PARAM INTEGER                                 {Analyz::Instance.currentFunc().CallParam($2,0);}
| Symbol '=' CALL FUNCTION                    {Analyz::Instance.currentFunc().CallFunc($1,$4);}
| RETURN Symbol                               {Analyz::Instance.currentFunc().ReturnFunc($2,1);}
| RETURN INTEGER                              {Analyz::Instance.currentFunc().ReturnFunc($2,0);}
| LABEL ':'                                     {$$=new Expression(Label,{},{},{$1});
                                                Analyz::Instance.labelTable[$1] = $$;
}
;
Symbol:
VARIABLE                                        {$$ = $1;}
|PARAMVARIABLE                                  {$$ = Analyz::Instance.currentFunc().paramTable[$1];}

%%