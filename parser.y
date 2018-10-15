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
    namespace Output
    {
        std::vector<std::string> list;
        int gen(std::string s)
        {
            list.push_back(s);
            return list.size() - 1;
        }
        void print()
        {
            for(std::string s : list)
            {
                std::cout<<s;
            }
        }
        void patch(int i,Label& l,int x =0)
        {
            if(x ==0)
            list.at(i) += l.print() + "\n";
            else list.at(i) = l.print() + ":\n";
        }
    }
    int Symbol::origCount = 0;
    int Symbol::tempCount = 0;
    int Label::usedCount = 0;
}



%token EQUAL NOTEQUAL LAND LOR T_INT MAIN IF ELSE WHILE RETURN
%token<number> INTEGER
%token<name> IDENTIFIER
%type<node> Goal BeforeMain BeforeMainStatement VarDefn VarDecls VarDecl FuncDefn FuncCreateIdTable
%type<node> InsideFuncStatements FuncDecl MainFunc Statements Statement Expression Params Identifier
%type<node> M N
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
Goal: BeforeMain MainFunc           {Output::print();}
;
BeforeMain: BeforeMain BeforeMainStatement
|   %empty
;
BeforeMainStatement: VarDefn | FuncDefn | FuncDecl
;
VarDefn: Type Identifier ';'    {   $$ = new Node($2,NULL,NodeType::Vardfn,0);
                                    $2->sym->Declear($1);
                                    $2->sym->Define();
                                    // printf("var T%d\n",$2->sym->id);
                                    std::stringstream ss;
                                    ss<<"var T"<<$2->sym->id<<"\n";
                                    Output::gen(ss.str());
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
                                            $1->sym->print();//printf(" [%d]\n",$2->val);
                                            std::stringstream ss;
                                            ss<<" ["<< $2->val <<"]\n";
                                            Output::gen(ss.str());
 }
'{'    
Statements M
'}'    { /* delete top; */ top = save;
         $$ = new Node($1,$6,NodeType::Fundfn,$2->val); 
         /* printf("end ");  */ 
         Output::gen("end ");$1->sym->print();Output::gen("\n");
}
;
InsideFuncStatements: InsideFuncStatements M FuncDecl
| InsideFuncStatements M Statement
| %empty
;
FuncDecl: FuncCreateIdTable VarDecls ')' ';'
        {/* delete top; */ top = save;}
;
MainFunc: T_INT MAIN '(' ')' '{' Statements M'}'
;
Type: T_INT   {$$ = SymbolType::Int;}
;
Statements: Statements M Statement      { // std::cerr<<"GAY"<<std::endl;
                                            $$ = new Node($1,$3,NodeType::Stmts,0);
                                            $1->nextlist.backpatch($2->instr);
                                            $$->nextlist = $3->nextlist;
}
//| %empty                             { $$= new Node(NULL,NULL,NodeType::Stmts,0);}
| Statement {$$ = $1;}
;
M: %empty { $$ = new Node(NULL,NULL,NodeType::Empty,0); 
            $$->instr.Init(Output::gen(":"));
}
;
N: %empty { $$ = new Node(NULL,NULL,NodeType::Empty,0);
            $$->nextlist = Gotolist(Output::gen("goto "));
}
;
Statement: '{'    {save = top;top = new SymbolTable(top);}
            Statements '}'        {/* delete top; */ top = save;
                                     $$ = $3;
}      
| IF '('  Expression ')' M Statement {   $3->truelist.backpatch($5->instr);
                                         $$ = new Node($3,$6,NodeType::If,0);
                                         $$->nextlist = $3->falselist.merge($6->nextlist);
                                        
}
| IF '('  Expression ')' M Statement N ELSE M Statement  {
                                        $$ = new Node($3,$6,NodeType::IfElse,0);
                                        $3->truelist.backpatch($5->instr);
                                        $3->falselist.backpatch($9->instr);
                                        $$->nextlist = $6->nextlist.merge($10->nextlist);
                                        $$->nextlist = $$->nextlist.merge($7->nextlist);
}
| WHILE M '(' Expression ')' M Statement           {
                                        $$ = new Node($4,$7,NodeType::While,0);
                                        $7->nextlist.backpatch($2->instr);
                                        $4->truelist.backpatch($6->instr);
                                        $$->nextlist = $4->falselist;
                                        Output::gen("goto "+$2->instr.print());
                                        
}
| Identifier '=' Expression ';'           {$$ =new Node($1,$3,NodeType::Assign,'=');
                                            $1->sym->print();
                                            Output::gen(" = ");//printf(" = ");
                                            $3->sym->print();
                                            Output::gen("\n");//printf("\n");

}
| Identifier '[' Expression ']' '=' Expression ';'
| VarDefn
| RETURN Expression ';'                 {$$ = new Node($2,NULL,NodeType::Return,0);
                                            Output::gen("return ");$2->sym->print();Output::gen("\n");}
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
                                        //    $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"<");
                                            Output::gen("if ");$1->sym->print();Output::gen(" < ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
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
| Expression LAND M Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LAND);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"&&");
                                           $1->truelist.backpatch($3->instr);
                                           $$->truelist = $4->truelist;
                                           $$->falselist = $1->falselist.merge($4->falselist);
}
| Expression LOR M Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LOR);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"||");
                                           $1->falselist.backpatch($3->instr);
                                           $$->falselist = $4->falselist;
                                           $$->truelist = $1->truelist.merge($4->truelist);
                                            
}
| Expression '[' Expression ']'               {$$ = new Node($1,$3,NodeType::DualArith,'[');
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");$1->sym->print();
                                                Output::gen(" [");$3->sym->print();Output::gen("]\n");
                                                $$->sym = tmpsym;
}
| INTEGER                                     {$$ = new Node(NULL,NULL,NodeType::Symbol1,$1);
                                                $$->sym = new Symbol(SymbolType::Immediate,$1);
}
| Identifier                                  {if($1->sym->decleared == false) {$1->sym->ReportError("Undecleared");}
    $$ = $1;}
| '!' Expression                              {$$ = new Node($2,NULL,NodeType::SingleLogic,'!');
                                                // $$->sym = Symbol::ProcessSingleOp($2->sym,"!");
                                                $$->falselist = $2->truelist;
                                                $$->truelist = $2->falselist;
} 
| '-' Expression %prec NEGA                   {$$ = new Node($2,NULL,NodeType::SingleArith,'-');
                                                $$->sym = Symbol::ProcessSingleOp($2->sym,"-");
}
| Identifier '(' Params ')'                   {$$ = new Node($1,$3,NodeType::Funcall,$3->val);
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");
                                                Output::gen("call ");$1->sym->print();Output::gen("\n");
                                                $$->sym = tmpsym;
                                                
}
| '(' Expression ')'                          {$$ = $2;
}
;
Params: Params ',' Expression       {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1);
                                        Output::gen("param ");$3->sym->print();Output::gen("\n");
}
|   Expression                         { $$=new Node(NULL,$1,NodeType::Params,1);
                                        Output::gen("param ");$1->sym->print();Output::gen("\n");
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
                                        sym->funName = (char*)malloc(4+strlen($1));
                                        strcpy(sym->funName,"f_");
                                        strcat(sym->funName,$1);
                                        $$->sym = sym;
}
;

%%

int yyerror(const char *msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<std::endl;
return 0;
}