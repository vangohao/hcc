#include<iostream>
#include<sstream>
#include<string.h>
#include<map>
#include<string>
#include<vector>
#include "symbol.h"
struct Node;
namespace Output
{
int gen(std::string);
}
Symbol::Symbol()
    {
        id = -1;type = SymbolType::Error;val = -1;tempid=-1;
        appear = NULL;decleared = false;defined = false;paramCount = -1;
    }
    Symbol::Symbol(Node* _appear)
    {
        appear = _appear;
        decleared = false;
        defined = false;
        paramCount = -1;
        tempid=-1;
    }
    Symbol::Symbol(SymbolType _type) //创建临时符号
    {
        tempid = tempCount++;
        type = _type;
        paramCount = -1;
        id = -1;
        decleared = true;
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
        defined = true;
    }
    void Symbol::print()
    {
        std::stringstream ss;
        if(type==SymbolType::FunPtr)
        {
            ss<<funName;
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
    Symbol* Symbol::ProcessDualOp(Symbol* s1,Symbol* s2,const char * x)
    {
        Symbol* tmpsym = new Symbol(SymbolType::Int);
        if(s1->type != SymbolType::Int && s1->type != SymbolType::Immediate)
        {
            std::string str = s1->funName;
            ReportError("Identifier " + str.substr(2,str.length()-2) + " is not a number  !");
        }
        else if(s2->type != SymbolType::Int && s2->type != SymbolType::Immediate)
        {
            std::string str = s2->funName;
            ReportError("Identifier " + str.substr(2,str.length()-2) + " is not a number  !");
        }
        // printf("var ");tmpsym->print();putchar('\n');
        // tmpsym->print();printf(" = ");s1->print();
        // printf(" %s ",x);s2->print();putchar('\n');
        Output::gen("var ");tmpsym->print();Output::gen("\n");
        tmpsym->print();Output::gen(" = ");s1->print();
        Output::gen(" ");Output::gen(x);Output::gen(" ");
        s2->print();Output::gen("\n");
        return tmpsym;
    }
    Symbol* Symbol::ProcessSingleOp(Symbol* s1,const char * x)
    {
        Symbol* tmpsym = new Symbol(SymbolType::Int);
        // printf("var ");tmpsym->print();putchar('\n');
        // tmpsym->print();printf(" = ");
        // printf("%s",x);s1->print();putchar('\n');
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
            ReportError("Redecleared");
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
        else if(_val != val || type!=_type)
        {
            ReportError("Function Defined does not match Decleared");
        }
        Define();
    }
    void Symbol::Define()
    {
        if(defined==false)
            defined = true;
        else
        {
            ReportError("Redefined");
        }
    }
    void Symbol::ReportError(std::string s)
    {
        std::cerr<<s<<std::endl;
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