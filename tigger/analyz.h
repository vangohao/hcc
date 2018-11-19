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
/* struct Variable
{
    int val;
    bool param;
    int pid;
    Variable(int _val,bool _param=false,int _pid=0);
}; */
class Expression
{
public:
    ExprType type;
    bool isMove;
    vector<int> left;
    vector<int> right;
    vector<int> imm;
    vector<int> use;
    vector<int> def;
    vector<int> in;
    vector<int> in1;
    vector<int> out;
    vector<int> out1;
    vector<Expression*> nexts;
    vector<Expression*> prevs;
    string funtocall;
    string funin;
    Expression(ExprType _type,std::initializer_list<int> _left,
    std::initializer_list<int> _right,std::initializer_list<int> _imm,string _funtocall="",string _funin="");
};
class Func
{
public:
    string name;
    int paramCount;
    int frameSize;
    vector<int> offset;
    vector<int> size;
    vector<int> paramTable;
    map<int,int> paramTableReverse;
    vector<Expression*> exprs;
    void insert(int s);
    Func(int _paramCount,string _name);
    void genFlow();
    void livelyAnalyz();
    void DebugPrint();
    void DebugPrint(vector<int> & v);
    
private:
};
class Analyz
{
public:
    static int vcount;
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