#include "analyz.h"
/* Variable::Variable(int _val,bool _param,int _pid):
    val(_val),param(_param),pid(_pid){} */
Expression::Expression(ExprType _type,std::initializer_list<int> _left,
        std::initializer_list<int> _right,
        std::initializer_list<int> _imm,string _funtocall,string _funin)
:type(_type),left(_left),right(_right),
imm(_imm),funtocall(_funtocall),funin(_funin)
{
    Analyz::Instance.currentFunc().exprs.push_back(this);
    if(_type == MoveRR)
    {
        this->isMove = true;
    }
    else
    {
        this->isMove = false;
    }
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
Func::Func(int _paramCount,string _name):paramCount(_paramCount),name(_name)
{
    for(int i = 0;i<_paramCount;i++)
    {
        paramTable.push_back(++Analyz::vcount);
        paramTableReverse[Analyz::vcount] = i;
    }
}
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
    
    for(auto e:exprs)
    {
        e->use = e->right;
        e->def = e->left;
        std::sort(e->use.begin(),e->use.end());
        std::sort(e->def.begin(),e->def.end());
    }
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
        }
        if(flag) break;
    }while(1);
    DebugPrint();
}
void Func::DebugPrint()
{
    cerr<<name<<endl;
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
void Func::initColorAlgorithm()
{
    //initialize Vectors
    int maxVariable = 0;
    for(auto e:exprs)
    {
        for(auto v:e->use)
        {
            if(v > maxVariable)
            {
                maxVariable = v;
            }
        }
    }
    adjMatrix = vector<vector<int>>(maxVariable + 10);
    adjList = vector<list<int>>(maxVariable + 10);
    degrees = vector<int>(maxVariable+10);
    alias = vector<int>(maxVariable+10);
    color = vector<int>(maxVariable+10);
    for(int i = 0; i<= maxVariable; i++)
    {
        adjMatrix[i] = vector<int>(maxVariable+10);
    }
    vector<int> tmp(maxVariable+10);
    for(auto e:exprs)
    {
        for(auto v:e->use)
        {
            tmp[v] = 1;
        }
    }
    for(int v = 0; v<= maxVariable; v++)
    {
        if(tmp[v])
        {
            initial.push_back(v);
        }
    }


    //BuildMatrix
    for(auto e:exprs)
    {
        if(e->isMove)
        {
            moveList[e->def[0]].push_back(e);
            moveList[e->use[0]].push_back(e);
            worklistMoves.push_back(e);
            for(auto i = e->in.begin();i!=e->in.end(); i++)
            {
                for(auto j = i+1; j!= e->in.end(); j++)
                {
                    if((*i == e->def[0] && *j== e->use[0]) || (*i==e->use[0] && *j == e->def[0]))
                    {
                        
                    }
                    else
                    {
                        AddEdge(*i,*j);
                    }
                }
            }
        }
        for(auto i = e->in.begin();i!=e->in.end(); i++)
        {
            for(auto j = i+1; j!= e->in.end(); j++)
            {
                AddEdge(*i,*j);
            }
        }
    }

    //MakeWorklist
    for(int v: initial)
    {
        if(degrees[v] >= colorNumber)
        {
            spillWorklist.push_back(v);
            status[v] = Hard;
        }
        else if(MoveRelated(v))
        {
            freezeWorklist.push_back(v);
            status[v] = Freeze;
        }
        else 
        {
            simplifyWorklist.push_back(v);
            status[v] = Simple;
        }
    }
}
void Func::AddEdge(int x,int y)
{
    if(adjMatrix[x][y] == 0)
    {
        adjMatrix[x][y] = 1;
        adjMatrix[y][x] = 1;
        adjList[x].push_back(y);
        adjList[y].push_back(x);
        degrees[x] ++;
        degrees[y] ++;
    }
}
bool Func::MoveRelated(int n)
{
    return NodeMoves(n).size() != 0;
}
list<Expression*>& Func::NodeMoves(int n)
{
    list<Expression*> *tmp = new list<Expression*>;
    for(auto e:moveList[n])
    {
        bool yes = false;
        for(auto i: activeMoves)
        {
            if(i == e) yes = true;
        }
        for(auto i: worklistMoves)
        {
            if(i == e) yes = true;
        }
        if(yes) tmp->push_back(e);
    }
    return *tmp;
}
list<int>& Func::Adjacent(int n)
{
    list<int> *tmp = new list<int>;
    for(auto v:adjList[n])
    {
        if(status[v] == Stacked || status[v] == Coalesced)
        {
        }
        else tmp->push_back(v);
    }
    return *tmp;
}
void Func::Simplify()
{
    int n = simplifyWorklist.front();
    simplifyWorklist.pop_front();
    selectStack.push_back(n);
    status[n] = Stacked;
    for(auto m: Adjacent(n))
    {
        DecrementDegree(m);
    }
}
void Func::DecrementDegree(int m)
{
    int d = degrees[m];
    degrees[m] --;
    if(d==colorNumber)
    {
        EnableMoves(m);
        for(auto v:Adjacent(m))
        {
            EnableMoves(v);
        }
        spillWorklist.remove(m);
        if(MoveRelated(m))
        {
            freezeWorklist.push_back(m);
            status[m] = Freeze;
        }
        else
        {
            simplifyWorklist.push_back(m);
            status[m] = Simple;
        }
    }
}
void Func::EnableMoves(int m)
{
    for(auto e:NodeMoves(m))
    {
        if(activeMoves.end() != std::find(activeMoves.begin(),activeMoves.end(),e))
        {
            activeMoves.remove(e);
            worklistMoves.push_back(e);    
        }
    }
}
void Func::Coalesce()
{
    auto e = worklistMoves.front();
    worklistMoves.pop_front();
    int x = e->def[0];
    int y = e->use[0];
    x = GetAlias(x);
    y = GetAlias(y);
    int u;
    int v;
    if(status[y] == Precolored)
    {
        u = y;
        v = x;
    }
    else
    {
        u = x;
        v = y;
    }
    if( u==v)
    {
        coalescedMoves.push_back(e);
        AddWorklist(u);
    }
    else if(status[v] == Precolored || adjMatrix[u][v])
    {
        constrainedMoves.push_back(e);
        AddWorklist(u);
        AddWorklist(v);
    }
    else if((status[u]==Precolored && TestPrecoloredCombine(u,v))
    ||(status[u]!=Precolored && TestConservative(u,v)))
    {
        coalescedMoves.push_back(e);
        Combine(u,v);
        AddWorklist(u);
    }
    else 
    {
        activeMoves.push_back(e);
    }
}
void Func::AddWorklist(int u)
{
    if(status[u]!= Precolored && !MoveRelated(u) && degrees[u] < colorNumber)
    {
        freezeWorklist.remove(u);
        simplifyWorklist.push_back(u);
        status[u] = Simple;
    }
}
bool Func::TestPrecoloredCombine(int u/*precolored*/,int v)
{
    for(auto x: Adjacent(v))
    {
        if(!(degrees[x]<colorNumber || status[x]==Precolored
                || adjMatrix[x][u]))
        {
            return false;
        }
    }
    return true;
}
bool Func::TestConservative(int u,int v)
{
    int k = 0;
    for(auto x: Adjacent(u))
    {
        if(degrees[x] >= colorNumber) k++;
    }
    for(auto x: Adjacent(v))
    {
        if(degrees[x] >= colorNumber) k++;
    }
    return k<colorNumber;
}
int Func::GetAlias(int x)
{
    if(status[x] == Coalesced) return GetAlias(alias[x]);
    else return x;
}
void Func::Combine(int u,int v)
{
    if(status[v] == Freeze)
    {
        freezeWorklist.remove(v);
    }
    else 
    {
        spillWorklist.remove(v);
    }
    coalescedNodes.push_back(v);
    status[v] = Coalesced;
    alias[v] = u;
    moveList[u].insert(moveList[u].end(),moveList[v].begin(),moveList[v].end());
    EnableMoves(v);
    for(auto t:Adjacent(v))
    {
        AddEdge(t,u);
        DecrementDegree(t);
    }
    if(degrees[u] >= colorNumber && status[u] ==Freeze)
    {
        freezeWorklist.remove(u);
        spillWorklist.push_back(u);
        status[u] = Hard;
    }
}
void Func::FreezeAction()
{
    int u = freezeWorklist.front();
    freezeWorklist.pop_front();
    simplifyWorklist.push_back(u);
    status[u]= Simple;
    FreezeMoves(u);
}
void Func::FreezeMoves(int u)
{
    for(auto e: NodeMoves(u))
    {
        int x = e->def[0];
        int y = e->use[0];
        int v;
        if(GetAlias(y)== GetAlias(u))
        {
            v = GetAlias(x);
        }
        else
        {
            v = GetAlias(y);
        }
        activeMoves.remove(e);
        frozenMoves.push_back(e);
        if(NodeMoves(v).size()==0 && degrees[v] < colorNumber)
        {
            freezeWorklist.remove(v);
            simplifyWorklist.push_back(v);
            status[v] = Simple;
        } 
    }
}
void Func::SelectSpill()
{
    //选择一个启发式算法找出下一个溢出的节点,此处暂时选择队头的节点
    int m = spillWorklist.front();
    spillWorklist.pop_front();
    simplifyWorklist.push_back(m);
    status[m]  = Simple;
    FreezeMoves(m);
}
void Func::AssignColors()
{
    while(selectStack.size()!=0)
    {
        int n = selectStack.back();
        selectStack.pop_back();
        vector<int> colorsToUse(colorNumber);
        for(auto w:adjList[n])
        {
            if(status[GetAlias(w)] == Colored || status[GetAlias(w)] == Precolored)
            {
                colorsToUse[color[GetAlias(w)]] = 1;
            }
        }
        int i= 0;
        for(;i!=colorNumber && colorsToUse[i] == 0;i++);
        if(i == colorNumber)
        {
            spilledNodes.push_back(n);
            status[n] =Spilled;
        }
        else
        {
            coloredNodes.push_back(n);
            color[n] = i;
        }
    }
    for(auto n: coalescedNodes)
    {
        color[n] = color[GetAlias(n)];
    }
}
void Func::RewriteProgram()
{
    
}
int Func::colorNumber = 27;