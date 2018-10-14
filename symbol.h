#include<iostream>
#include<sstream>
#include<map>
#include<string>
#include<vector>
struct Node;
namespace Output
{
int gen(std::string);
}
enum SymbolType //符号类型
{
    Int=0,
    IntPtr=1,
    FunPtr=2,
    Immediate=3,
    Error=4,
};
struct Symbol //符号
{
    static int origCount;
    static int tempCount;
    int id;
    int tempid;
    SymbolType type;
    int val; //附加值，函数参数个数或数组大小
    Node* appear; //第一次出现的位置
    bool decleared;
    bool defined;
    int paramCount;
    int immediate;
    char * funName; //函数名称
    Symbol()
    {
        id = -1;type = SymbolType::Error;val = -1;tempid=-1;
        appear = NULL;decleared = false;defined = false;paramCount = -1;
    }
    Symbol(Node* _appear)
    {
        appear = _appear;
        decleared = false;
        defined = false;
        paramCount = -1;
        tempid=-1;
    }
    Symbol(SymbolType _type) //创建临时符号
    {
        tempid = tempCount++;
        type = _type;
        paramCount = -1;
        id = -1;
        decleared = true;
        defined = true;
    }
    Symbol(SymbolType _type, int _immediate) //创建直接数符号
    {
        type=_type;
        immediate = _immediate;
        paramCount = -1;
        id =-1;
        tempid = -1;
        decleared = true;
        defined = true;
    }
    void print()
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
    static Symbol* ProcessDualOp(Symbol* s1,Symbol* s2,const char * x)
    {
        Symbol* tmpsym = new Symbol(SymbolType::Int);
        // printf("var ");tmpsym->print();putchar('\n');
        // tmpsym->print();printf(" = ");s1->print();
        // printf(" %s ",x);s2->print();putchar('\n');
        Output::gen("var ");tmpsym->print();Output::gen("\n");
        tmpsym->print();Output::gen(" = ");s1->print();
        Output::gen(" ");Output::gen(x);Output::gen(" ");
        s2->print();Output::gen("\n");
        return tmpsym;
    }
    static Symbol* ProcessSingleOp(Symbol* s1,const char * x)
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
    void Declear(SymbolType _type,int _paramCount=-1, int _val=0)
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
    void Define()
    {
        if(defined==false)
            defined = true;
        else
        {
            ReportError("Redefined");
        }
    }
    static void ReportError(const char * s)
    {
        std::cerr<<s<<std::endl;
    }
};

struct SymbolTable
{
    std::map<std::string,Symbol*> table;
    SymbolTable * prev;
    SymbolTable(SymbolTable * st)
    {
        prev = st;
    }
    void put(const char * s, Symbol* sym)
    {
        table.insert(std::make_pair(s,sym));
    }
    Symbol* get(const char * s)
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
};