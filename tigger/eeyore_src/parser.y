%code requires{
    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<string>
    #include<map>
    #include<stack>
    #include "symbol.h"
    #include "node.h"
    #include "fault.h"

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
%{
    #define YYERROR_VERBOSE 1
%}

%code{
    extern FILE* yyout;
    extern int yylineno;
    SymbolTable * top = new SymbolTable(NULL); 
    std::stack<SymbolTable*> save;
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
                fprintf(yyout,"%s",s.c_str());
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



%token EQUAL NOTEQUAL LAND LOR T_INT MAIN IF ELSE WHILE RETURN IllegalCharacter
%token<number> INTEGER
%token<name> IDENTIFIER
%type<node> Goal BeforeMainStatement VarDefn VarDecls VarDecl FuncDefn FuncCreateIdTable Ifhead
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
| error ';'                                 {
                                            exit(1);
}
;
BeforeMain: BeforeMain BeforeMainStatement
|   %empty
;
BeforeMainStatement: VarDefn | FuncDefn | FuncDecl | ';' {$$=NULL;}
;
VarDefn: Type Identifier ';'    {   $$ = new Node($2,NULL,NodeType::Vardfn,0,yylineno);
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear($1);
                                    $2->sym->Define();
                                    std::stringstream ss;
                                    ss<<"var T"<<$2->sym->id<<"\n";
                                    Output::gen(ss.str());
                                }
| Type Identifier'['INTEGER']' ';'  {
                                    $$ = new Node($2,NULL,NodeType::Vardfn,4 * $4,yylineno);
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear(SymbolType::IntPtr,-1,4 * $4);
                                    $2->sym->Define();
                                    std::stringstream ss;
                                    ss<<"var "<<4 * $4<<" T"<<$2->sym->id<<"\n";
                                    Output::gen(ss.str());
}
;
VarDecls:VarDecls ',' VarDecl   {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1,yylineno);
                                    $3->sym->paramCount = $1->val;
                                    $3->sym->Define();
}
| VarDecl                        {
                                    $$ = new Node(NULL,$1,NodeType::Params,1,yylineno);
                                    $1->sym->paramCount = 0;
                                    $1->sym->Define();
} 
| %empty                      {$$ = new Node(NULL,NULL,NodeType::Params,0,yylineno);
}
;
VarDecl: Type Identifier        {  
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear($1,0);
                                    $$=$2;
}
| Type Identifier '[' INTEGER ']' {  
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear(SymbolType::IntPtr,0,4 * $4);
                                    $$=$2;
}
| Type Identifier '[' ']'          {   
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear(SymbolType::IntPtr,0,0);
                                    $$ = $2;
}
;
FuncCreateIdTable: Type Identifier '(' {
                                        if($2->val == 1)  //该符号在最内层程序体中未定义.
                                        {
                                            Symbol * sym = new Symbol($2);
                                            top->put($2->sym->funName,sym);
                                            $2->sym = sym;
                                        }
                                        save.push(top);top = new SymbolTable(top);/*函数前的创建符号表*/
                                        $$ = $2;
}
;
FuncDefn: FuncCreateIdTable VarDecls ')'    { $1->sym->Define(SymbolType::FunPtr,0,$2->val);
                                            $1->sym->DefineParamList($2);  //传入参数node表
                                            $1->sym->print();
                                            std::stringstream ss;
                                            ss<<" ["<< $2->val <<"]\n";
                                            Output::gen(ss.str());
 }
'{'    
InsideFuncStatements M
'}'    {   delete top;  top = save.top();save.pop();//恢复符号表
         $$ = new Node($1,$6,NodeType::Fundfn,$2->val,yylineno); 
         $6->nextlist.backpatch($7->instr);
         Output::gen("end ");$1->sym->print();Output::gen("\n");
}
;
InsideFuncStatements: InsideFuncStatements M FuncDecl        { 
                                            $$ = new Node($1,$3,NodeType::Stmts,0,yylineno);
                                            $1->nextlist.backpatch($2->instr);
}
| InsideFuncStatements M Statement                          { 
                                            $$ = new Node($1,$3,NodeType::Stmts,0,yylineno);
                                            $1->nextlist.backpatch($2->instr);
                                            $$->nextlist = $3->nextlist;
}
| FuncDecl                      {$$ = $1;}
| Statement                     {$$ = $1;}
;
FuncDecl: FuncCreateIdTable VarDecls ')' ';'
        {   $$ = new Node($1,$2,NodeType::Fundcl,$2->val,yylineno);
            $1->sym->Declear(SymbolType::FunPtr,0,$2->val);
            $1->sym->DefineParamList($2);  //传入参数node表
            delete top;  top = save.top(); save.pop();
}
;
MainFunc: T_INT MAIN '(' ')'            {   save.push(top);top = new SymbolTable(top);/*函数前的创建符号表*/ 
                                            Output::gen("f_main [0]\n");
}
 '{' InsideFuncStatements M'}'                    {   
                                            $7->nextlist.backpatch($8->instr);
                                            Output::gen("end f_main\n");
                                            delete top;
                                            top = save.top(); save.pop(); //恢复符号表
}
;
Type: T_INT   {$$ = SymbolType::Int;}
;
Statements: Statements M Statement      { 
                                            $$ = new Node($1,$3,NodeType::Stmts,0,yylineno);
                                            $1->nextlist.backpatch($2->instr);
                                            $$->nextlist = $3->nextlist;
}
//| %empty                             { $$= new Node(NULL,NULL,NodeType::Stmts,0,yylineno);}
| Statement {$$ = $1;}
;
M: %empty { $$ = new Node(NULL,NULL,NodeType::Empty,0,yylineno); 
            $$->instr.Init(Output::gen(""));
}
;
N: %empty { $$ = new Node(NULL,NULL,NodeType::Empty,0,yylineno);
            $$->nextlist = Gotolist(Output::gen("goto "));
}
;
Ifhead:  IF '('  Expression ')'    {   if($3->type != NodeType::ExprLogic)
                                            {
                                                Output::gen("if ");
                                                $3->sym->print();
                                                $3->truelist = Gotolist(Output::gen(" != 0 goto "));
                                                $3->falselist = Gotolist(Output::gen("goto "));
                                            }
                                            $$ = $3;
}
;
Statement: '{'    {save.push(top);top = new SymbolTable(top);}
            Statements '}'        { delete top;  top = save.top(); save.pop();
                                     $$ = $3;
}      
| ';'           {$$ = new Node(NULL,NULL,NodeType::Stmts,0);}
| Ifhead M Statement {  
                                        $$ = new Node($1,$3,NodeType::If,0,yylineno);
                                        $1->truelist.backpatch($2->instr);
                                        $$->nextlist = $1->falselist.merge($3->nextlist);
}
| Ifhead M Statement N ELSE M Statement  {
                                        $$ = new Node($1,$3,NodeType::IfElse,0,yylineno);
                                        $1->truelist.backpatch($2->instr);
                                        $1->falselist.backpatch($6->instr);
                                        $$->nextlist = $3->nextlist.merge($7->nextlist);
                                        $$->nextlist = $$->nextlist.merge($4->nextlist);
}
| WHILE M '(' Expression ')'      {
                                        if($4->type != NodeType::ExprLogic)
                                        {
                                            Output::gen("if ");
                                            $4->sym->print();
                                            $4->truelist = Gotolist(Output::gen(" != 0 goto "));
                                            $4->falselist = Gotolist(Output::gen("goto "));
                                        }
}
 M Statement           {
                                        $$ = new Node($4,$8,NodeType::While,0,yylineno);
                                        $8->nextlist.backpatch($2->instr);
                                        $4->truelist.backpatch($7->instr);
                                        $$->nextlist = $4->falselist;
                                        Gotolist(Output::gen("goto ")).backpatch($2->instr);
}
| Identifier '=' Expression ';'           {$$ =new Node($1,$3,NodeType::Assign,'=',yylineno);
                                            //处理逻辑表达式赋值
                                            if($3->type == NodeType::ExprLogic)
                                            {
                                                Label l1;
                                                l1.Init(Output::gen(""));
                                                $1->sym->print();
                                                Output::gen(" = 1\n");
                                                Label l2;
                                                l2.Init(Output::gen(""));
                                                $1->sym->print();
                                                Output::gen(" = 0\n");
                                                $3->truelist.backpatch(l1);
                                                $3->falselist.backpatch(l2);
                                            }
                                            else
                                            {
                                                $1->sym->print();
                                                Output::gen(" = ");
                                                $3->sym->print();
                                                Output::gen("\n");
                                            }
}
| Identifier '[' Expression ']' '='  {
                                        if($3->sym->type == Immediate)
                                        {
                                            $3->sym->immediate *= 4;
                                        }
                                        else
                                        {
                                        Node * tmpnode = new Node(NULL,NULL,Symbol1,0,yylineno);
                                        tmpnode -> sym = new Symbol(SymbolType::Immediate,4);
                                        $3->sym = Symbol::ProcessDualOp($3,tmpnode,"*");
                                        }

}
 Expression ';'  {$$ =new Node($1,$3,NodeType::ArrayAssign,'=',yylineno);
                                            if($7->type == NodeType::ExprLogic)
                                            {
                                                Label l1;
                                                l1.Init(Output::gen(""));
                                                $1->sym->print();
                                                Output::gen(" [");
                                                $3->sym->print();
                                                Output::gen("] = 1\n");
                                                Label l2;
                                                l2.Init(Output::gen(""));
                                                $1->sym->print();
                                                Output::gen(" [");
                                                $3->sym->print();
                                                Output::gen("] = 0\n");
                                                $7->truelist.backpatch(l1);
                                                $7->falselist.backpatch(l2);
                                            }
                                            else
                                            {
                                                $1->sym->print();
                                                Output::gen(" [");
                                                $3->sym->print();
                                                Output::gen("] = ");
                                                $7->sym->print();
                                                Output::gen("\n");
                                            }
}
| Identifier '(' Params ')' ';' {
                                            $$ = new Node($1,$3,NodeType::Funcall,$3->val,yylineno);
                                            Symbol* tmpsym = new Symbol(SymbolType::Int);
                                            $1->sym->CallWithParams($3);
                                            Output::gen("var ");tmpsym->print();Output::gen("\n");
                                            tmpsym->print();Output::gen(" = ");
                                            Output::gen("call ");$1->sym->print();Output::gen("\n");
                                            $$->sym = tmpsym;
}
| VarDefn
| RETURN Expression ';'                 {$$ = new Node($2,NULL,NodeType::Return,0,yylineno);
                                            
                                            if($2->type == NodeType::ExprLogic)
                                            {
                                                Label l1;
                                                l1.Init(Output::gen(""));
                                                Output::gen("return 1\n");
                                                Label l2;
                                                l2.Init(Output::gen(""));
                                                Output::gen("return 0\n");
                                                $2->truelist.backpatch(l1);
                                                $2->falselist.backpatch(l2);
                                            }
                                            else if($2->sym->type != Int && $2->sym->type !=Immediate)
                                            {
                                                yyerror("Return expression is not an int");
                                                if(errorstatus) {YYERROR;}
                                            }
                                            else{
                                                Output::gen("return ");$2->sym->print();Output::gen("\n");
                                            }
}
| error ';'                                 {
                                            yyclearin;
                                            errorstatus = false;
                                            exit(1);
}
;
Expression:  Expression '+' Expression    {$$ = new Node($1,$3,NodeType::ExprArith,'+',yylineno);
                                           $$->sym = Symbol::ProcessDualOp($1,$3,"+");
                                           if(errorstatus) {YYERROR;}
}
| Expression '-' Expression               {$$ = new Node($1,$3,NodeType::ExprArith,'-',yylineno);
                                           $$->sym = Symbol::ProcessDualOp($1,$3,"-");
                                           if(errorstatus) {YYERROR;}
}
| Expression '*' Expression               {$$ = new Node($1,$3,NodeType::ExprArith,'*',yylineno);
                                           $$->sym = Symbol::ProcessDualOp($1,$3,"*");
                                           if(errorstatus) {YYERROR;}
}
| Expression '/' Expression               {$$ = new Node($1,$3,NodeType::ExprArith,'/',yylineno);
                                           $$->sym = Symbol::ProcessDualOp($1,$3,"/");
                                           if(errorstatus) {YYERROR;}
}
| Expression '%' Expression               {$$ = new Node($1,$3,NodeType::ExprArith,'%',yylineno);
                                           $$->sym = Symbol::ProcessDualOp($1,$3,"%");
                                           if(errorstatus) {YYERROR;}
}
| Expression '<' Expression               {$$ = new Node($1,$3,NodeType::ExprLogic,'<',yylineno);
                                            Output::gen("if ");$1->sym->print();Output::gen(" < ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
                                            
}
| Expression '>' Expression               {$$ = new Node($1,$3,NodeType::ExprLogic,'>',yylineno);
                                           Output::gen("if ");$1->sym->print();Output::gen(" > ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
                                            
}
| Expression EQUAL Expression             {$$ = new Node($1,$3,NodeType::ExprLogic,EQUAL,yylineno);
                                           Output::gen("if ");$1->sym->print();Output::gen(" == ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
                                            
}
| Expression NOTEQUAL Expression          {$$ = new Node($1,$3,NodeType::ExprLogic,NOTEQUAL,yylineno);
                                           Output::gen("if ");$1->sym->print();Output::gen(" != ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
                                            
}
| Expression LAND           {
                                            if($1->type != NodeType::ExprLogic)
                                            {
                                                Output::gen("if ");$1->sym->print();
                                                $1->truelist = Gotolist(Output::gen(" != 0 goto "));
                                                $1->falselist = Gotolist(Output::gen("goto "));
                                            }
}
M Expression               {$$ = new Node($1,$5,NodeType::ExprLogic,LAND,yylineno);
                                           if($5->type != NodeType::ExprLogic)
                                            {
                                                Output::gen("if ");$5->sym->print();
                                                $5->truelist = Gotolist(Output::gen(" != 0 goto "));
                                                $5->falselist = Gotolist(Output::gen("goto "));
                                            }                 
                                           $1->truelist.backpatch($4->instr);
                                           $$->truelist = $5->truelist;
                                           $$->falselist = $1->falselist.merge($5->falselist);
                                            
}
| Expression LOR           {
                                            if($1->type != NodeType::ExprLogic)
                                            {
                                                Output::gen("if ");$1->sym->print();
                                                $1->truelist = Gotolist(Output::gen(" != 0 goto "));
                                                $1->falselist = Gotolist(Output::gen("goto "));
                                            }
}
M Expression               {$$ = new Node($1,$5,NodeType::ExprLogic,LOR,yylineno);
                                            if($5->type != NodeType::ExprLogic)
                                            {
                                                Output::gen("if ");$5->sym->print();
                                                $5->truelist = Gotolist(Output::gen(" != 0 goto "));
                                                $5->falselist = Gotolist(Output::gen("goto "));
                                            }   
                                            $1->falselist.backpatch($4->instr);
                                            $$->falselist = $5->falselist;
                                            $$->truelist = $1->truelist.merge($5->truelist);
                                            
                                            
}
| Expression '[' Expression ']'               {$$ = new Node($1,$3,NodeType::ExprArith,'[',yylineno);
                                                if($1->sym->type != IntPtr)
                                                {
                                                    yyerror("Expression before \'[\' is not an int pointer");
                                                    if(errorstatus) {YYERROR;}
                                                }
                                                else
                                                {
                                                    if($3->sym->type == Immediate)
                                                    {
                                                    Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                    Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                    tmpsym->print();Output::gen(" = ");$1->sym->print();
                                                    $3->sym->immediate *= 4;
                                                    Output::gen(" [");$3->sym->print();Output::gen("]\n");
                                                    $$->sym = tmpsym;
                                                    }
													else
													{
                                                    Node * tmpnode = new Node(NULL,NULL,Symbol1,0,yylineno);
                                                    tmpnode -> sym = new Symbol(SymbolType::Immediate,4);
                                                    Symbol * tmpans = Symbol::ProcessDualOp($3,tmpnode,"*");
                                                    Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                    Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                    tmpsym->print();Output::gen(" = ");$1->sym->print();
                                                    Output::gen(" [");tmpans->print();Output::gen("]\n");
                                                    $$->sym = tmpsym;
													}
                                                }
}
| INTEGER                                     {$$ = new Node(NULL,NULL,NodeType::Symbol1,$1,yylineno);
                                                $$->sym = new Symbol(SymbolType::Immediate,$1);
}
| Identifier                                  {if($1->sym->decleared == false) {$1->sym->ReportUndecleared();}
                                                $$ = $1;
                                                if(errorstatus) {YYERROR;}
    }
| '!' Expression                              {$$ = new Node($2,NULL,NodeType::ExprLogic,'!',yylineno);
                                                if($2->type != NodeType::ExprLogic)
                                                {
                                                    Output::gen("if ");$2->sym->print();
                                                    $2->truelist = Gotolist(Output::gen(" != 0 goto "));
                                                    $2->falselist = Gotolist(Output::gen("goto "));
                                                }   
                                                $$->falselist = $2->truelist;
                                                $$->truelist = $2->falselist;
                                                
} 
| '-' Expression %prec NEGA                   {$$ = new Node($2,NULL,NodeType::ExprArith,'-',yylineno);
                                                $$->sym = Symbol::ProcessSingleOp($2,"-");
}
| Identifier '(' Params ')'                   {
                                                $$ = new Node($1,$3,NodeType::Funcall,$3->val,yylineno);
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                $1->sym->CallWithParams($3);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");
                                                Output::gen("call ");$1->sym->print();Output::gen("\n");
                                                $$->sym = tmpsym;
                                                if(errorstatus) {YYERROR;}
}
| '(' Expression ')'                          {$$ = $2;
}
;
Params: Params ',' Expression       {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1,yylineno);
                                        Output::gen("param ");$3->sym->print();Output::gen("\n");
}
|   Expression                         { $$=new Node(NULL,$1,NodeType::Params,1,yylineno);
                                        Output::gen("param ");$1->sym->print();Output::gen("\n");
}
| %empty        {$$ = new Node(NULL,NULL,NodeType::Params,0);}
;
Identifier: IDENTIFIER             {    $$ = new Node(NULL,NULL,NodeType::Symbol1,0,yylineno);
                                        Symbol * sym = top->get($1);
                                        Symbol * here = top->gethere($1);
                                        if(sym == NULL)
                                        {
                                            sym = new Symbol($$);
                                            top->put($1,sym);
                                        }
                                        else if(here == NULL)
                                        {
                                            $$->val = 1;//标记该标识符在最内层函数中未定义。
                                        }
                                        sym->funName = $1;
                                        $$->sym = sym;
}
;

%%

