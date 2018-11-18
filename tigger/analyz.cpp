#include "analyz.h"
Expression::Expression(ExprType _type,std::initializer_list<int> _def,
        std::initializer_list<int> _use,std::initializer_list<int> _imm,string _funtocall,string _funin)
:type(_type),def(_def),use(_use),imm(_imm),funtocall(_funtocall),funin(_funin)
{
    Analyz::Instance.currentFunc().exprs.push_back(this);
}
void Analyz::insert(int s)
{
    offset.push_back(globalSize);
    size.push_back(s);
    globalSize += s;
}
void Func::insert(int s)
{
    offset.push_back(frameSize);
    size.push_back(s);
    frameSize += s;
}
Func::Func(int _paramCount,string _name):paramCount(_paramCount),name(_name){}
Func& Analyz::currentFunc()
{
    return *(funcs.rbegin());
}