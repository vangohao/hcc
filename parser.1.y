%code requires{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<map>
    #include "hcc.h"
    int yylex();  
    int yyparse();
    int yyerror(const char *);
    typedef union{
        int number;
        char* name;
        SymbolType type;
        Node* node;
    } foryystype;
    #define YYSTYPE foryystype
}

%code{
    SymbolTable * top = new SymbolTable(NULL); 
    int Symbol::count = 0;
}



%token EQUAL NOTEQUAL LAND LOR T_INT MAIN IF ELSE WHILE RETURN
%token<number> INTEGER
%token<name> IDENTIFIER
%type<node> Goal BeforeMain BeforeMainStatement VarDefn VarDecls VarDecl FuncDefn
%type<node> InsideFuncStatements FuncDecl MainFunc Statements Statement Expression Params Identifier
%type<type> Type
%right '='
%left LAND LOR
%left EQUAL NOTEQUAL
%left '<' '>'
%left '+' '-'
%left '*' '/' '%'
%right NEGA '!'
%right '[' 

%%
Goal: BeforeMain MainFunc
;
BeforeMain: BeforeMain BeforeMainStatement
|   %empty
;
BeforeMainStatement: VarDefn | FuncDefn | FuncDecl
;
VarDefn: Type Identifier ';'    {   $$ = new Node($2,NULL,NodeType::Vardfn,0);
                                    Symbol* sym = top->get($2->name);
                                    if(sym)
                                    {
                                        sym->Declear($1);
                                        sym->Define();
                                    }
                                }
| Type Identifier'['INTEGER']' ';'
;
VarDecls:VarDecls ',' VarDecl
| %empty
;
VarDecl: Type Identifier
| Type Identifier '[' INTEGER ']'
| Type Identifier '[' ']'
;
FuncDefn: Type Identifier '(' VarDecls ')' '{' InsideFuncStatements '}'
;
InsideFuncStatements: InsideFuncStatements FuncDecl
| InsideFuncStatements Statement
| %empty
;
FuncDecl: Type Identifier '(' VarDecls ')' ';'
;
MainFunc: T_INT MAIN '(' ')' '{' InsideFuncStatements '}'
;
Type: T_INT
;
Statements: Statements Statement
| %empty
;
Statement: '{' Statements '}'              {
                                                SymbolTable * save = top;
                                                top = new SymbolTable(top);

}
| IF '(' Expression ')' Statement
| IF '(' Expression ')' Statement ELSE Statement
| WHILE '(' Expression ')' Statement
| Identifier '=' Expression ';'
| Identifier '[' Expression ']' '=' Expression ';'
| VarDefn
| RETURN Expression ';'
;
Expression:  Expression '+' Expression    {$$ = new Node($1,$3,NodeType::DualArith,'+');}
| Expression '-' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'-');}
| Expression '*' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'*');}
| Expression '/' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'/');}
| Expression '%' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'%');}
| Expression '<' Expression               {$$ = new Node($1,$3,NodeType::DualLogic,'<');}
| Expression '>' Expression               {$$ = new Node($1,$3,NodeType::DualLogic,'>');}
| Expression EQUAL Expression             {$$ = new Node($1,$3,NodeType::DualLogic,EQUAL);}
| Expression NOTEQUAL Expression          {$$ = new Node($1,$3,NodeType::DualLogic,NOTEQUAL);}
| Expression LAND Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LAND);}
| Expression LOR Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LOR);}
| Expression '[' Expression ']'               {$$ = new Node($1,$3,NodeType::DualArith,'[');}
| INTEGER                                     {$$ = new Node(NULL,NULL,NodeType::Immediate,$1);}
| Identifier                                  {$$ = $1;}
| '!' Expression                              {$$ = new Node($1,NULL,NodeType::SingleLogic,'!');} 
| '-' Expression %prec NEGA                   {$$ = new Node($1,NULL,NodeType::SingleArith,'-');}
| Identifier '(' Params ')'                   {$$ = new Node($1,$2,NodeType::Funcall,$2.val)}
| '(' Expression ')'                          {$$ = new Node($1,NULL,NodeType::Bracket,0)}
;
Params: Params ',' Identifier       {   $$ = new Node($1,$3,NodeType::Params,$1.val + 1);
}
|   %empty                          { $$=NULL;}
;
Identifier: IDENTIFIER             {    $$ = new Node(NULL,NULL,NodeType::Identifier,0);
                                        Symbol * sym = top->get($1);
                                        if(sym == NULL)
                                        {
                                            sym = new Symbol($$);
                                            top->put($1,sym);
                                        }
                                        $$->symbol = sym;
                                        
}
;

%%

int yyerror(const char *msg)            //输出错误信息的yyerror()函数
{
printf("Error encountered: %s \n", msg);
return 0;
}