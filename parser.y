%code requires{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<map>
    #include "symbol.h"
    #include "node.h"
    
    int yylex();  
    int yyparse();
    int yyerror(const char *);
    typedef struct{
        int number;
        char* name;
        SymbolType type;
        Node* node;
    } foryystype;
    #define YYSTYPE foryystype
}

%code{
    SymbolTable * top = new SymbolTable(NULL); 
    SymbolTable * save = NULL;
    int Symbol::origCount = 0;
    int Symbol::tempCount = 0;
    int Label::usedCount = 0;
}



%token EQUAL NOTEQUAL LAND LOR T_INT MAIN IF ELSE WHILE RETURN
%token<number> INTEGER
%token<name> IDENTIFIER
%type<node> Goal BeforeMain BeforeMainStatement VarDefn VarDecls VarDecl FuncDefn FuncCreateIdTable
%type<node> InsideFuncStatements FuncDecl MainFunc Statements Statement Expression Params Identifier
%type<type> Type
%right '='
%left LOR
%left LAND
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
                                    $2->sym->Declear($1);
                                    $2->sym->Define();
                                    printf("var T%d\n",$2->sym->id);
                                }
| Type Identifier'['INTEGER']' ';'
;
VarDecls:VarDecls ',' VarDecl   {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1);
                                    $3->sym->paramCount = $1->val;
                                    $3->sym->Define();
}
| VarDecl                        {
                                    $$ = new Node(NULL,$1,NodeType::Params,1);
                                    $1->sym->paramCount = 0;
                                    $1->sym->Define();
} 
| %empty                      {$$ = new Node(NULL,NULL,NodeType::Params,0);
}
;
VarDecl: Type Identifier        {   $$ = new Node($2,NULL,NodeType::VarDcl,0);
                                    $2->sym->Declear($1,0);
                                    $$->sym = $2->sym;
}
| Type Identifier '[' INTEGER ']'
| Type Identifier '[' ']'
;
FuncCreateIdTable: Type Identifier '(' {save = top;top = new SymbolTable(top);/*函数前的创建符号表*/
                                        $$ = $2;
}
;
FuncDefn: FuncCreateIdTable VarDecls ')'    { $1->sym->Declear(SymbolType::FunPtr,0,$2->val);
                                            $1->sym->Define();
                                            $1->sym->print();printf(" [%d]\n",$2->val);
 }
'{'    
InsideFuncStatements 
'}'    { /* delete top; */ top = save;
         $$ = new Node($1,$6,NodeType::Fundfn,$2->val); 
         printf("end ");  $1->sym->print();
}
;
InsideFuncStatements: InsideFuncStatements FuncDecl
| InsideFuncStatements Statement
| %empty
;
FuncDecl: FuncCreateIdTable VarDecls ')' ';'
        {/* delete top; */ top = save;}
;
MainFunc: T_INT MAIN '(' ')' '{' InsideFuncStatements '}'
;
Type: T_INT   {$$ = SymbolType::Int;}
;
Statements: Statements Statement
| %empty
;
IfBegin: IF '(' {

}
Statement: '{'    {save = top;top = new SymbolTable(top);}
            Statements '}'        {/* delete top; */ top = save;}      
| IfBegin Expression ')' Statement   
| IfBegin Expression ')' Statement ELSE Statement
| WHILE '(' Expression ')' Statement
| Identifier '=' Expression ';'           {$$ =new Node($1,$3,NodeType::Assign,'=');
                                            $1->sym->print();printf(" = ");$3->sym->print();printf("\n");

}
| Identifier '[' Expression ']' '=' Expression ';'
| VarDefn
| RETURN Expression ';'                 {$$ = new Node($2,NULL,NodeType::Return,0);
                                            printf("return ");$2->sym->print();}
;
Expression:  Expression '+' Expression    {$$ = new Node($1,$3,NodeType::DualArith,'+');
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"+");
}
| Expression '-' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'-');
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"-");
}
| Expression '*' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'*');
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"*");
}
| Expression '/' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'/');
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"/");
}
| Expression '%' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'%');
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"%%");
}
| Expression '<' Expression               {$$ = new Node($1,$3,NodeType::DualLogic,'<');
                                            if($$->type == NodeType::If || $$->type==NodeType::IfElse 
                                            || $$->type==NodeType::While)
                                            {
                                            printf("if ");$1->sym->print();printf(" < ");$3->sym->print();
                                            printf(" goto ");$$->ltrue->print();putchar('\n');
                                            printf(" goto ");$$->lfalse->print();putchar('\n');
                                            }
                                            else
                                                $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"<");
}
| Expression '>' Expression               {$$ = new Node($1,$3,NodeType::DualLogic,'>');
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,">");
}
| Expression EQUAL Expression             {$$ = new Node($1,$3,NodeType::DualLogic,EQUAL);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"==");
}
| Expression NOTEQUAL Expression          {$$ = new Node($1,$3,NodeType::DualLogic,NOTEQUAL);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"!=");
}
| Expression LAND Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LAND);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"&&");
}
| Expression LOR Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LOR);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"||");
                                            
}
| Expression '[' Expression ']'               {$$ = new Node($1,$3,NodeType::DualArith,'[');
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                printf("var ");tmpsym->print();putchar('\n');
                                                tmpsym->print();printf(" = ");$1->sym->print();
                                                printf(" [");$3->sym->print();printf("]\n");
                                                $$->sym = tmpsym;
}
| INTEGER                                     {$$ = new Node(NULL,NULL,NodeType::Symbol1,$1);
                                                $$->sym = new Symbol(SymbolType::Immediate,$1);
}
| Identifier                                  {if($1->sym->decleared == false) {$1->sym->ReportError("Undecleared");}
    $$ = $1;}
| '!' Expression                              {$$ = new Node($2,NULL,NodeType::SingleLogic,'!');
                                                $$->sym = Symbol::ProcessSingleOp($2->sym,"!");
} 
| '-' Expression %prec NEGA                   {$$ = new Node($2,NULL,NodeType::SingleArith,'-');
                                                $$->sym = Symbol::ProcessSingleOp($2->sym,"-");
}
| Identifier '(' Params ')'                   {$$ = new Node($1,$3,NodeType::Funcall,$3->val);
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                printf("var ");tmpsym->print();putchar('\n');
                                                tmpsym->print();printf(" = ");
                                                printf("call ");$1->sym->print();putchar('\n');
                                                $$->sym = tmpsym;
                                                
}
| '(' Expression ')'                          {$$ = $2;
}
;
Params: Params ',' Expression       {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1);
                                        printf("param ");$3->sym->print();putchar('\n');
}
|   Expression                         { $$=new Node(NULL,$1,NodeType::Params,1);
                                        printf("param ");$1->sym->print();putchar('\n');
}
| %empty        {$$ = new Node(NULL,NULL,NodeType::Params,0);}
;
Identifier: IDENTIFIER             {    $$ = new Node(NULL,NULL,NodeType::Symbol1,0);
                                        Symbol * sym = top->get($1);
                                        if(sym == NULL)
                                        {
                                            sym = new Symbol($$);
                                            top->put($1,sym);
                                        }
                                        sym->funName = strdup("f_");
                                        strcat(sym->funName,$1);
                                        $$->sym = sym;
                                        
}
;

%%

int yyerror(const char *msg)            //输出错误信息的yyerror()函数
{
printf("Error encountered: %s \n", msg);
return 0;
}