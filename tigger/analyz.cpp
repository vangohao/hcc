#include "analyz.h"
Expression::Expression(ExprType _type,std::initializer_list<int> _left,
        std::initializer_list<int> _right,
        std::initializer_list<int> _imm,string _funtocall,string _funin)
:type(_type),left(_left),right(_right),
imm(_imm),funtocall(_funtocall),funin(_funin),isMove(0)
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
void Analyz::process()
{
    for(auto f: funcs)
    {
        f.genFlow();
        f.livelyAnalyz();
    }
}
void Func::genFlow()
{
    for(auto e:exprs)
    {
        if(e->type==Goto)
        {
            e->nexts.push_back(Analyz::Instance.labelTable[e->imm[0]]);
        }
        else if(e->type==IfRR || e->type==IfRI || e->type==IfIR)
        {
            e->nexts.push_back(Analyz::Instance.labelTable[e->imm[1]]);
        }
    }
    for(auto e:exprs)
    {
        for(auto n: e->nexts)
        {
            n->prevs.push_back(e);
        }
    }
}
void Func::livelyAnalyz()
{
    
        DebugPrint();
    for(auto e:exprs)
    {
        e->use = e->right;
        e->def = e->left;
        std::sort(e->use.begin(),e->use.end());
        std::sort(e->def.begin(),e->def.end());
    }
        DebugPrint();
    do
    {
        bool flag = true;
        for(auto e:exprs)
        {
            e->in1 = e->in;
            e->out1 = e->out;
            vector<int> tmp;
            auto p1 = e->out.begin();
            auto p2 = e->def.begin();
            while(p1!=e->out.end() && p2!=e->def.end())
            {
                while(p2!=e->def.end() && *p2 < *p1) p2++;
                if(p2!=e->def.end() && *p2 > *p1)
                {
                    tmp.push_back(*p1);
                    p1++;
                }
                else if(p2!=e->def.end() && *p2 == *p1)
                {
                    p1++;
                }
            }
            if(p1!=e->out.end())
            {
                tmp.insert(tmp.end(),p1,e->out.end());
            }
            e->in.clear();
            p1 = e->use.begin();
            p2 = tmp.begin();
            while(p1!=e->use.end() && p2 != tmp.end())
            {
                if(*p1 <*p2) {e->in.push_back(*p1);p1++;}
                else if(*p2 <*p1) {e->in.push_back(*p2);p2++;}
                else {e->in.push_back(*p1); p1++; p2++;}
            }
            if(p1!=e->use.end())
            {
                e->in.insert(e->in.end(),p1,e->use.end());
            }
            else{e->in.insert(e->in.end(),p2,tmp.end());}
            e->out.clear();
            for(Expression* expr: e->nexts)
            {
                e->out.insert(e->out.end(),expr->in.begin(),expr->in.end());
            }
            std::sort(e->out.begin(),e->out.end());
            e->out.resize(std::unique(e->out.begin(),e->out.end()) - e->out.begin());
            if(e->in != e->in1 || e->out != e->out1)
            {
                flag = false;
            }
        DebugPrint();
        }
        if(flag) break;
    }
    while(1);
}
void Func::DebugPrint()
{
    for(auto e: exprs)
    {
        cerr<<"def :";DebugPrint(e->def);
        cerr<<"use :";DebugPrint(e->use);
        cerr<<"in :";DebugPrint(e->in);
        cerr<<"out :";DebugPrint(e->out);
    }
    cerr<<"---------------"<<endl;
}
void Func::DebugPrint(vector<int> & v)
{
    for(auto x: v)
    {
        cerr<<x<<" ";
    }
    cerr<<endl;
}