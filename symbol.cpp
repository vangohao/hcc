#include<iostream>
#include<sstream>
#include<string.h>
#include<map>
#include<string>
#include<vector>
#include "symbol.h"
#include "node.h"
#include "fault.h"
namespace Output
{
int gen(std::string);
}
Symbol::Symbol()
{
    id = -1;type = SymbolType::Error;val = -1;tempid=-1;paramlist=NULL;
    appear = NULL;decleared = false;defined = false;paramCount = -1;
}
Symbol::Symbol(Node* _appear)
{
    appear = _appear;
    decleared = false;
    defined = false;
    paramCount = -1;
    paramlist=NULL;
    tempid=-1;
}
Symbol::Symbol(SymbolType _type) //创建临时符号
{
    tempid = tempCount++;
    type = _type;
    paramCount = -1;
    id = -1;
    decleared = true;
    paramlist=NULL;
    defined = true;
}
Symbol::Symbol(SymbolType _type, int _immediate) //创建直接数符号
{
    type=_type;
    immediate = _immediate;
    paramCount = -1;
    id =-1;
    tempid = -1;
    decleared = true;
    paramlist=NULL;
    defined = true;
}
void Symbol::print()
{
    std::stringstream ss;
    if(type==SymbolType::FunPtr)
    {
        ss<<"f_"<<funName;
    }
    else if(type==SymbolType::Immediate)
    {
        ss<<immediate;
    }
    else if(id!=-1)
    {
        ss<<"T"<<id;
    }
    else if(tempid==-1)
    {
        ss<<"p"<<paramCount;
    }
    else
    {
        ss<<"t"<<tempid;
    }
    Output::gen(ss.str());
}
Symbol* Symbol::ProcessDualOp(Node* n1,Node* n2,const char * x)
{
    Symbol* s1 = n1->sym;
    Symbol* s2 = n2->sym;
    if(n1->type == ExprLogic)
    {
        Symbol* tmpsym1 = new Symbol(SymbolType::Int);
        Label l1;
        l1.Init(Output::gen(""));
        Output::gen("var ");tmpsym1->print();Output::gen("\n");
        tmpsym1->print();
        Output::gen(" = 1\n");
        Label l2;
        l2.Init(Output::gen(""));
        tmpsym1->print();
        Output::gen(" = 0\n");
        n1->truelist.backpatch(l1);
        n1->falselist.backpatch(l2);
        s1 = tmpsym1;
    }
    if(n2->type == ExprLogic)
    {
        Symbol* tmpsym1 = new Symbol(SymbolType::Int);
        Label l1;
        l1.Init(Output::gen(""));
        Output::gen("var ");tmpsym1->print();Output::gen("\n");
        tmpsym1->print();
        Output::gen(" = 1\n");
        Label l2;
        l2.Init(Output::gen(""));
        tmpsym1->print();
        Output::gen(" = 0\n");
        n2->truelist.backpatch(l1);
        n2->falselist.backpatch(l2);
        s2 = tmpsym1;
    }
    Symbol* tmpsym = new Symbol(SymbolType::Int);
    if(s1->type != SymbolType::Int && s1->type != SymbolType::Immediate)
    {
        yyerror((std::string)"Expression for "+x+" is not a number");
    }
    else if(s2->type != SymbolType::Int && s2->type != SymbolType::Immediate)
    {
        yyerror((std::string)"Expression for "+x+" is not a number");
    }
    Output::gen("var ");tmpsym->print();Output::gen("\n");
    tmpsym->print();Output::gen(" = ");s1->print();
    Output::gen(" ");Output::gen(x);Output::gen(" ");
    s2->print();Output::gen("\n");
    return tmpsym;
}
Symbol* Symbol::ProcessSingleOp(Node* n1,const char * x)
{
    Symbol* s1 = n1->sym;
    if(n1->type == ExprLogic)
    {
        Symbol* tmpsym1 = new Symbol(SymbolType::Int);
        Label l1;
        l1.Init(Output::gen(""));
        Output::gen("var ");tmpsym1->print();Output::gen("\n");
        tmpsym1->print();
        Output::gen(" = 1\n");
        Label l2;
        l2.Init(Output::gen(""));
        tmpsym1->print();
        Output::gen(" = 0\n");
        n1->truelist.backpatch(l1);
        n1->falselist.backpatch(l2);
        s1 = tmpsym1;
    }
    Symbol* tmpsym = new Symbol(SymbolType::Int);
    if(s1->type != SymbolType::Int && s1->type != SymbolType::Immediate)
    {
        yyerror((std::string)"Expression for "+x+" is not a number");
    }
    Output::gen("var ");tmpsym->print();Output::gen("\n");
    tmpsym->print();
    Output::gen(" = ");Output::gen(x);Output::gen(" ");
    s1->print();Output::gen("\n");
    return tmpsym;
}
void Symbol::Declear(SymbolType _type,int _paramCount, int _val)
{
    if(decleared == false){
        if(_paramCount == -1)
        {
            id = origCount++;
        }
        else id = -1;
        type = _type;
        val = _val;
        decleared = true;}
    else 
    {
        yyerror((std::string)"Redecleared");
    }
}
void Symbol::Define(SymbolType _type,int _paramCount, int _val)
{
    if(decleared == false){
        if(_paramCount == -1)
        {
            id = origCount++;
        }
        else id = -1;
        type = _type;
        val = _val;
        decleared = true;}
    else if(type!=_type)
    {
        yyerror((std::string)"Identifier" + funName +" Defined as a different type");
    }
    Define();
}
void Symbol::DefineParamList(Node * pl)
{
    if(  !paramlist|| checkParams(pl))
    {
        paramlist = pl;
    }
    else
    {
        yyerror((std::string)"Function " + funName + " Defined does not match its declear");
    }
}
void Symbol::Define()
{
    if(defined==false)
        defined = true;
    else
    {
        yyerror((std::string)"Identifier " + funName + " Redefined");
    }
}
void Symbol::CallWithParams(Node * n1)
{
    if(decleared == true && type != FunPtr)
    {
        yyerror((std::string)"Identifier " + funName + " decleared as not a function");
        return;
    }
    if(decleared == false)
    {
    //	yyerror((std::string)"Function " + funName + " is not decleared");
        Declear(FunPtr,0,n1->val);
        DefineParamList(n1);
    }
    if(decleared == true && !checkParams(n1))
    {
        yyerror((std::string)"Call of function " + funName + " has wrong params");
    }
}
bool Symbol::checkParams(Node * n1)
{
    Node * n0 = paramlist;
    if(! (n0->right) && !(n1->right)) return true;
    while (n0 && n1)
    {
        if(!((n0->right->sym->type == SymbolType::Int && n1->right->sym->type == SymbolType::Immediate)
            ||
            (n1->right->sym->type == SymbolType::Int && n0->right->sym->type == SymbolType::Immediate)  
            )&&
            (n0->right->sym->type != n1->right->sym->type))
            {
            return false;
            }
        n0 = n0->left;
        n1 = n1->left;
    }
    if(n0 || n1)
    {
        return false;
    }
    else return true;
}
void Symbol::ReportUndecleared()
{
    yyerror((std::string)"Identifier " + funName + " not decleared"); 
}




SymbolTable::SymbolTable(SymbolTable * st)
{
    prev = st;
}
void SymbolTable::put(const char * s, Symbol* sym)
{
    table.insert(std::make_pair(s,sym));
}
Symbol* SymbolTable::get(const char * s)
{
    for(SymbolTable* t =this; t!=NULL; t = t->prev)
    {
        auto it = t->table.find(s);
        if(it!=t->table.end())
        {
            return it->second;
        }
    }
    return NULL;
}
Symbol* SymbolTable::gethere(const char * s)
{
    auto it = this->table.find(s);
    if(it!=this->table.end())
    {
        return it->second;
    }
    return NULL;
}