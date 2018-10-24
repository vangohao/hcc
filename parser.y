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



%token EQUAL NOTEQUAL LAND LOR T_INT MAIN IF ELSE WHILE RETURN
%token<number> INTEGER
%token<name> IDENTIFIER
%type<node> Goal BeforeMainStatement VarDefn VarDecls VarDecl FuncDefn FuncCreateIdTable
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
BeforeMainStatement: VarDefn | FuncDefn | FuncDecl | ';'
;
VarDefn: Type Identifier ';'    {   $$ = new Node($2,NULL,NodeType::Vardfn,0,linenum);
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
                                    $$ = new Node($2,NULL,NodeType::Vardfn,4 * $4,linenum);
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
VarDecls:VarDecls ',' VarDecl   {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1,linenum);
                                    $3->sym->paramCount = $1->val;
                                    $3->sym->Define();
}
| VarDecl                        {
                                    $$ = new Node(NULL,$1,NodeType::Params,1,linenum);
                                    $1->sym->paramCount = 0;
                                    $1->sym->Define();
} 
| %empty                      {$$ = new Node(NULL,NULL,NodeType::Params,0,linenum);
}
;
VarDecl: Type Identifier        {   //$$ = new Node($2,NULL,NodeType::VarDcl,0,linenum);
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        // std::cerr<<tmp->funName +2<<std::endl;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear($1,0);
                                    //$$->sym = $2->sym;
                                    $$=$2;
}
| Type Identifier '[' INTEGER ']' {   //$$ = new Node($2,NULL,NodeType::VarDcl,4 * $4,linenum);
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear(SymbolType::IntPtr,0,4 * $4);
                                    //$$->sym = $2->sym;
                                    $$=$2;
}
| Type Identifier '[' ']'          {   //$$ = new Node($2,NULL,NodeType::VarDcl,0,linenum);
                                    if($2->val == 1)
                                    {
                                        Symbol * tmp = $2->sym;
                                        $2->sym = new Symbol($2);
                                        $2->sym->funName = tmp->funName;
                                        top->put(tmp->funName.c_str(),$2->sym);
                                    }
                                    $2->sym->Declear(SymbolType::IntPtr,0,0);
                                    // $$->sym = $2->sym;
                                    $$ = $2;
}
;
FuncCreateIdTable: Type Identifier '(' {save.push(top);top = new SymbolTable(top);/*函数前的创建符号表*/
                                        $$ = $2;
}
;
FuncDefn: FuncCreateIdTable VarDecls ')'    { $1->sym->Define(SymbolType::FunPtr,0,$2->val);
                                            $1->sym->DefineParamList($2);  //传入参数node表
                                            $1->sym->print();//printf(" [%d]\n",$2->val);
                                            std::stringstream ss;
                                            ss<<" ["<< $2->val <<"]\n";
                                            Output::gen(ss.str());
 }
'{'    
InsideFuncStatements M
'}'    {   delete top;  top = save.top();save.pop();//恢复符号表
         $$ = new Node($1,$6,NodeType::Fundfn,$2->val,linenum); 
         $6->nextlist.backpatch($7->instr);
         Output::gen("end ");$1->sym->print();Output::gen("\n");
}
;
InsideFuncStatements: InsideFuncStatements M FuncDecl        { 
                                            $$ = new Node($1,$3,NodeType::Stmts,0,linenum);
                                            $1->nextlist.backpatch($2->instr);
}
| InsideFuncStatements M Statement                          { 
                                            $$ = new Node($1,$3,NodeType::Stmts,0,linenum);
                                            $1->nextlist.backpatch($2->instr);
                                            $$->nextlist = $3->nextlist;
}
| FuncDecl                      {$$ = $1;}
| Statement                     {$$ = $1;}
;
FuncDecl: FuncCreateIdTable VarDecls ')' ';'
        {     $$ = new Node($1,$2,NodeType::Fundcl,$2->val,linenum);
            $1->sym->Declear(SymbolType::FunPtr,0,$2->val);
            $1->sym->DefineParamList($2);  //传入参数node表
            delete top;  top = save.top(); save.pop();}
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
                                            $$ = new Node($1,$3,NodeType::Stmts,0,linenum);
                                            $1->nextlist.backpatch($2->instr);
                                            $$->nextlist = $3->nextlist;
}
//| %empty                             { $$= new Node(NULL,NULL,NodeType::Stmts,0,linenum);}
| Statement {$$ = $1;}
;
M: %empty { $$ = new Node(NULL,NULL,NodeType::Empty,0,linenum); 
            $$->instr.Init(Output::gen(""));
}
;
N: %empty { $$ = new Node(NULL,NULL,NodeType::Empty,0,linenum);
            $$->nextlist = Gotolist(Output::gen("goto "));
}
;
Statement: '{'    {save.push(top);top = new SymbolTable(top);}
            Statements '}'        { delete top;  top = save.top(); save.pop();
                                     $$ = $3;
}      
| ';'           {$$ = new Node(NULL,NULL,NodeType::Stmts,0);}
| IF '('  Expression ')' M Statement {   $3->truelist.backpatch($5->instr);
                                         $$ = new Node($3,$6,NodeType::If,0,linenum);
                                         $$->nextlist = $3->falselist.merge($6->nextlist);
                                        
}
| IF '('  Expression ')' M Statement N ELSE M Statement  {
                                        $$ = new Node($3,$6,NodeType::IfElse,0,linenum);
                                        $3->truelist.backpatch($5->instr);
                                        $3->falselist.backpatch($9->instr);
                                        $$->nextlist = $6->nextlist.merge($10->nextlist);
                                        $$->nextlist = $$->nextlist.merge($7->nextlist);
}
| WHILE M '(' Expression ')' M Statement           {
                                        $$ = new Node($4,$7,NodeType::While,0,linenum);
                                        $7->nextlist.backpatch($2->instr);
                                        $4->truelist.backpatch($6->instr);
                                        $$->nextlist = $4->falselist;
                                        Gotolist(Output::gen("goto ")).backpatch($2->instr);
                                        // Output::gen("goto "+$2->instr.print());
                                        
}
| Identifier '=' Expression ';'           {$$ =new Node($1,$3,NodeType::Assign,'=',linenum);
                                            $1->sym->print();
                                            Output::gen(" = ");
                                            $3->sym->print();
                                            Output::gen("\n");
}
| Identifier '[' Expression ']' '='  {
                                        Symbol * four = new Symbol(SymbolType::Immediate,4);
                                        $3->sym = Symbol::ProcessDualOp(four,$3->sym,"*");

}
 Expression ';'  {$$ =new Node($1,$3,NodeType::ArrayAssign,'=',linenum);
                                            $1->sym->print();
                                            Output::gen(" [");
                                            $3->sym->print();
                                            Output::gen("] = ");
                                            $7->sym->print();
                                            Output::gen("\n");
}
| VarDefn
| RETURN Expression ';'                 {$$ = new Node($2,NULL,NodeType::Return,0,linenum);
                                            Output::gen("return ");$2->sym->print();Output::gen("\n");}
;
Expression:  Expression '+' Expression    {$$ = new Node($1,$3,NodeType::DualArith,'+',linenum);
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"+");
}
| Expression '-' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'-',linenum);
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"-");
}
| Expression '*' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'*',linenum);
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"*");
}
| Expression '/' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'/',linenum);
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"/");
}
| Expression '%' Expression               {$$ = new Node($1,$3,NodeType::DualArith,'%',linenum);
                                           $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"%");
}
| Expression '<' Expression               {$$ = new Node($1,$3,NodeType::DualLogic,'<',linenum);
                                        //    $$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"<");
                                            Output::gen("if ");$1->sym->print();Output::gen(" < ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
}
| Expression '>' Expression               {$$ = new Node($1,$3,NodeType::DualLogic,'>',linenum);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,">");
                                           Output::gen("if ");$1->sym->print();Output::gen(" > ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
}
| Expression EQUAL Expression             {$$ = new Node($1,$3,NodeType::DualLogic,EQUAL,linenum);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"==");
                                           Output::gen("if ");$1->sym->print();Output::gen(" == ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
}
| Expression NOTEQUAL Expression          {$$ = new Node($1,$3,NodeType::DualLogic,NOTEQUAL,linenum);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"!=");
                                           Output::gen("if ");$1->sym->print();Output::gen(" != ");
                                            $3->sym->print();
                                            $$->truelist = Gotolist(Output::gen(" goto "));
                                            $$->falselist = Gotolist(Output::gen("goto "));
}
| Expression LAND M Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LAND,linenum);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"&&");
                                           $1->truelist.backpatch($3->instr);
                                           $$->truelist = $4->truelist;
                                           $$->falselist = $1->falselist.merge($4->falselist);
}
| Expression LOR M Expression               {$$ = new Node($1,$3,NodeType::DualLogic,LOR,linenum);
                                           //$$->sym = Symbol::ProcessDualOp($1->sym,$3->sym,"||");
                                           $1->falselist.backpatch($3->instr);
                                           $$->falselist = $4->falselist;
                                           $$->truelist = $1->truelist.merge($4->truelist);
                                            
}
| Expression '[' Expression ']'               {$$ = new Node($1,$3,NodeType::DualArith,'[',linenum);
                                                Symbol * four = new Symbol(SymbolType::Immediate,4);
                                                Symbol * tmpans = Symbol::ProcessDualOp(four,$3->sym,"*");
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");$1->sym->print();
                                                Output::gen(" [");tmpans->print();Output::gen("]\n");
                                                $$->sym = tmpsym;
}
| INTEGER                                     {$$ = new Node(NULL,NULL,NodeType::Symbol1,$1,linenum);
                                                $$->sym = new Symbol(SymbolType::Immediate,$1);
}
| Identifier                                  {if($1->sym->decleared == false) {$1->sym->ReportUndecleared();}
    $$ = $1;}
| '!' Expression                              {$$ = new Node($2,NULL,NodeType::SingleLogic,'!',linenum);
                                                // $$->sym = Symbol::ProcessSingleOp($2->sym,"!");
                                                $$->falselist = $2->truelist;
                                                $$->truelist = $2->falselist;
} 
| '-' Expression %prec NEGA                   {$$ = new Node($2,NULL,NodeType::SingleArith,'-',linenum);
                                                $$->sym = Symbol::ProcessSingleOp($2->sym,"-");
}
| Identifier '(' Params ')'                   {$$ = new Node($1,$3,NodeType::Funcall,$3->val,linenum);
                                                Symbol* tmpsym = new Symbol(SymbolType::Int);
                                                $1->sym->CallWithParams($3);
                                                Output::gen("var ");tmpsym->print();Output::gen("\n");
                                                tmpsym->print();Output::gen(" = ");
                                                Output::gen("call ");$1->sym->print();Output::gen("\n");
                                                $$->sym = tmpsym;
                                                
}
| '(' Expression ')'                          {$$ = $2;
}
;
Params: Params ',' Expression       {   $$ = new Node($1,$3,NodeType::Params,$1->val + 1,linenum);
                                        Output::gen("param ");$3->sym->print();Output::gen("\n");
}
|   Expression                         { $$=new Node(NULL,$1,NodeType::Params,1,linenum);
                                        Output::gen("param ");$1->sym->print();Output::gen("\n");
}
| %empty        {$$ = new Node(NULL,NULL,NodeType::Params,0);}
;
Identifier: IDENTIFIER             {    $$ = new Node(NULL,NULL,NodeType::Symbol1,0,linenum);
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

int yyerror(std::string msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<" at line "<<linenum<<"."<<std::endl;
Output::print();
return 0;
}
int yyerror(const char * msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<" at line "<<linenum<<"."<<std::endl;
Output::print();
return 0;
}