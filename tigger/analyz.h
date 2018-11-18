#ifndef __ANALYZ_H
#define __ANALYZ_H
#include "tigger.h"
enum ExprType
{
    Invalid,
    MoveRI,
    MoveRR,
    ArithRR,
    ArithRI,  //Allow + Only
    Negative,
    ArrayWrite,
    ArrayRead,
    IfRR,
    IfRI,
    IfIR,
    Goto,
    ReturnR,
    ReturnI,
    ParamR,
    ParamI,
    Empty,
    Call,
    Begin,
};
class Expression
{
public:
    ExprType type;
    vector<int> use;
    vector<int> def;
    vector<int> imm;
    vector<Expression*> nexts;
    int jmp;
    string funtocall;
    string funin;
    Expression(ExprType _type,std::initializer_list<int> _def,
    std::initializer_list<int> _use,std::initializer_list<int> _imm,string _funtocall="",string _funin="");
};
class Func
{
public:
    string name;
    int paramCount;
    int frameSize;
    vector<int> offset;
    vector<int> size;
    vector<Expression*> exprs;
    void insert(int s);
    Func(int _paramCount,string _name);
private:
};
class Analyz
{
public:
    static Analyz Instance;
    int globalSize;
    vector<int> offset;
    vector<int> size;
    std::map<string,Func*> FuncMap;
    std::map<int,Expression*> labelTable; 
    void insert(int s=4);
    vector<Func> funcs;
    Func& currentFunc();
    void process();
};

#endif