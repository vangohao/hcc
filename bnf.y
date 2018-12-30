%code requires{
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
Goal: BeforeMain MainFunc          
;
BeforeMain: BeforeMain BeforeMainStatement
|   %empty
;
BeforeMainStatement: VarDefn | FuncDefn | FuncDecl | ';'
;
VarDefn: Type Identifier ';'    
| Type Identifier'['INTEGER']' ';' 
;
VarDecls:VarDecls ',' VarDecl  
| VarDecl                      
| %empty                      
;
VarDecl: Type Identifier      
| Type Identifier '[' INTEGER ']'
| Type Identifier '[' ']'          
;
FuncCreateIdTable: Type Identifier '(' 
;
FuncDefn: FuncCreateIdTable VarDecls ')' 
'{'    
InsideFuncStatements M
'}'  
;
InsideFuncStatements: InsideFuncStatements M FuncDecl        
| InsideFuncStatements M Statement                          
| FuncDecl                     
| Statement                    
;
FuncDecl: FuncCreateIdTable VarDecls ')' ';'
;
MainFunc: T_INT MAIN '(' ')'           
 '{' InsideFuncStatements M'}'         
;
Type: T_INT  
;
Statements: Statements M Statement     
| Statement
;
M: %empty
;
N: %empty 
}
;
Ifhead:  IF '('  Expression ')'  
;
Statement: '{' Statements '}'       
| ';'          
| Ifhead M Statement 
| Ifhead M Statement N ELSE M Statement  
| WHILE M '(' Expression ')'  M Statement 
| Identifier '=' Expression ';' 
| Identifier '[' Expression ']' '='  Expression ';' 
| Identifier '(' Params ')' ';' 
| VarDefn
| RETURN Expression ';'         
| error ';'                     
;
Expression:  Expression '+' Expression 
| Expression '-' Expression            
| Expression '*' Expression            
| Expression '/' Expression            
| Expression '%' Expression           
| Expression '<' Expression           
| Expression '>' Expression           
| Expression EQUAL Expression       
| Expression NOTEQUAL Expression  
| Expression LAND M Expression       
| Expression LOR  M Expression  
| Expression '[' Expression ']' 
| INTEGER                       
| Identifier                    
| '!' Expression                
| '-' Expression %prec NEGA     
| Identifier '(' Params ')'     
| '(' Expression ')'            
;
Params: Params ',' Expression
| Expression               
| %empty   
;
Identifier: IDENTIFIER          

%%