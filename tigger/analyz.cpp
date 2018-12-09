#include "analyz.h"
#define REGNAMEFORVAR(x) (Reg::names[color[GetAlias((x))]])

extern unordered_map<int,int> frameArrayTable;
Analyz::Analyz()
{
    globalSize = 0; globalVariableCount = 0;
}
Expression::Expression(ExprType _type,vector<int> _left,
        vector<int> _right,
        vector<int> _imm,string _funtocall,string _funin,
        bool push)
:type(_type),left(_left),right(_right),
imm(_imm),funtocall(_funtocall),funin(_funin)
{
    if(_type == MoveRR)
    {
        this->isMove = true;
    }
    else
    {
        this->isMove = false;
    }
    if(push==false) return;
    bool leftGlobal = false;
    bool rightGlobal = false;
    for(int l:left)
    {
        if(Analyz::Instance.globalVaribleMap.find(l) != Analyz::Instance.globalVaribleMap.end())
        {
            leftGlobal = true;
            // cerr<<"LEFTGLOBAL_"<<l<<endl;
        }
    }
    for(auto iter = right.begin();iter!=right.end();iter++)
    {
        if(Analyz::Instance.globalVaribleMap.find(*iter) !=
         Analyz::Instance.globalVaribleMap.end())
        {
            // cerr<<"RIGHTGLOBAL_"<<*iter<<endl;
            //先load一下吧,有些语句可能不用load,以后再说.
            int tmp = Analyz::Instance.currentFunc().GenTempVariable();
            if(Analyz::Instance.globalVaribleType[*iter] == 0)
            Expression* e1 = new Expression(GlobalLoad,{tmp},{},{*iter});
            else
            Expression* e2 = new Expression(GlobalLoadAddr,{tmp},{},{*iter});
            *iter = tmp;
            rightGlobal = true;
        }
        else if(frameArrayTable.count(*iter))
        {
            int tmp = Analyz::Instance.currentFunc().GenTempVariable();
            Expression * e1 = new Expression(FrameLoadAddr,{tmp},{},
                {Analyz::Instance.currentFunc().offset[frameArrayTable[*iter]]});
            *iter = tmp;
        }
    }
    if(rightGlobal)
    {
        
    }
    if(leftGlobal)
    {
        int tmp1,tmp2;
        Expression* e1,* e2,* e3;
        switch(type)   //考虑到miniC转Eeyore时全局变量做左值只有赋值语句.
        {
            case MoveRI:
            tmp1 = Analyz::Instance.currentFunc().GenTempVariable();
            e1 = new Expression(GlobalLoadAddr,{tmp1},{},{left[0]});
            tmp2 = Analyz::Instance.currentFunc().GenTempVariable();
            left[0] = tmp2;
            Analyz::Instance.currentFunc().exprs.push_back(this);
            e3 = new Expression(ArrayWrite,{},{tmp1,tmp2},{0});
            break;
            case MoveRR:
            tmp1 = Analyz::Instance.currentFunc().GenTempVariable();
            e1 = new Expression(GlobalLoadAddr,{tmp1},{},{left[0]});
            e3 = new Expression(ArrayWrite,{},{tmp1,right[0]},{0});
            break;
        }
    }
    else
    {
        Analyz::Instance.currentFunc().exprs.push_back(this);
    }
    
}
vector<int> globalType;
void Analyz::insert(int var,int s,int type)
{
    // cerr<<globalVariableCount<<endl;
    //if(type == 0)cout<<"v"<<offset.size()<<" = 0"<<endl;
    //else cout<<"v"<<offset.size()<<" = malloc "<<s<<endl;
    offset.push_back(globalSize);
    size.push_back(s);
    globalSize += s;
    globalVaribleMap.insert(std::make_pair(var,globalVariableCount));
    globalVaribleType.insert(std::make_pair(var,type));
    globalType.push_back(type);
    globalVariableCount++;
}
void Analyz::GenGlobal()
{
    for(int i = 0; i<globalVariableCount; i++)
    {
        if(globalType[i])
        {
            cout<<".comm\tv"<<i<<","<<size[i]<<","<<8<<endl;
        }
        else
        {
            cout<<".global\tv"<<i<<endl;
            cout<<".section\t.sdata"<<endl;
            cout<<".align\t2"<<endl;
            cout<<".type\tv"<<i<<",@object"<<endl;
            cout<<".size\tv"<<i<<", 4"<<endl;
            cout<<"v"<<i<<":"<<endl;
            cout<<".word\t"<<0<<endl;
        }
    }
}
int Func::insert(int s,int v)
{
    int tmp = offset.size();
    offset.push_back(frameSize);
    size.push_back(s);
    frameSize += s;
    frameArrayTable[v] = tmp;
    return tmp;
}
int Func::insert()
{
    int tmp = offset.size();
    offset.push_back(frameSize);
    size.push_back(4);
    frameSize += 4;
    return tmp;
}
Func::Func(int _paramCount,string _name):paramCount(_paramCount),name(_name),paramToCallWithCount(0),frameSize(0)
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
void Func::InitFunEnv()//此函数处理函数入口和出口出的寄存器管理,不处理call语句和return语句
{
    if(!calledStoredRegs.empty()) return ;
    for(int i = 0; i<paramCount; i++)
    {
        int r = (int)(a0) + i;
        auto e= new Expression(MoveRR,{paramTable[i]},{r},{},"","",false);
        exprs.push_front(e);
    }
    if(name != "f_main")
    //保存被调用者保存的寄存器
    for(int i = 0; i<= 11; i++)
    {
        int r = (int)(s0) + i;
        int tmp1 = ++Analyz::vcount;
        calledStoredRegs.push_back(tmp1);
        auto e =  new Expression(MoveRR,{tmp1},{r},{},"","",false);
        exprs.push_front(e);
    }
}
void Analyz::process()
{
    for(auto f: funcs)
    {
        f.Processor();
    }
    GenGlobal();
}
void Func::genFlow()
{
    for(auto iter = exprs.begin();iter!=exprs.end();++iter)
    {
        Expression* e = *iter;
        iter++;
        e->nexts.clear();
        e->prevs.clear();
        if(iter != exprs.end())
        e->nexts.push_back(*iter);
        iter--;
        e->use = e->right;
        e->def = e->left;
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
void Func::ReturnFunc(int v,int t)
{
    if(calledStoredRegs.empty())
    {
        InitFunEnv();
    }
    if(name != "f_main")
    //恢复被调用者保存的寄存器
    for(int i = 0; i<= 11; i++)
    {
        int r = (int)(s0) + i;
        int tmp1 = calledStoredRegs[i];
        auto e =  new Expression(MoveRR,{r},{tmp1},{},"","",false);
        exprs.push_back(e);
    }
    //传送返回值到a0
    if(t == 1)
    {
        auto e = new Expression(MoveRR,{(int)(a0)},{v},{});
    }
    else
    {
        auto e = new Expression(MoveRI,{(int)(a0)},{},{v});
    }
    if(name != "f_main")
    //s开头的寄存器需要设为出口活跃以免冲突,a0也需要
    auto e = new Expression(Return,{},{(int)(a0),(int)(s0),(int)(s0)+1,(int)(s0)+2,(int)(s0)+3,
                                            (int)(s0)+4,(int)(s0)+5,(int)(s0)+6,(int)(s0)+7,(int)(s0)+8,(int)(s0)+9,
                                            (int)(s0)+10,(int)(s0)+11},{});
    else
    auto e = new Expression(Return,{},{(int)(a0)},{});
}
void Func::CallParam(int v,int t)
{
    if(t == 1)
    {
    auto e = new Expression(MoveRR,{(int)(a0)+paramToCallWithCount},{v},{});
    }
    else
    {
    auto e = new Expression(MoveRI,{(int)(a0)+paramToCallWithCount},{},{v});
    }
    paramToCallWithCount++;
}
void Func::CallFunc(int v,string f)
{
    if(f!="f_getint" && f!="f_putint" && f!="f_putchar")
    {
    vector<int> paramvec;
    for(int i = 0; i<paramToCallWithCount;i ++)
    {
        paramvec.push_back((int)(a0) + i);
    }
    for(int i = 0; i<=6;i ++)
    {
        paramvec.push_back((int)(t0) + i);
    }
    vector<int> tmpvec;
    //需要保存调用者保存的寄存器 包括a1-a9中不做参数的部分
     for(int i = 0; i<=6;i ++)
    {
        int r = (int)(t0) + i;
        int tmp1 = ++ Analyz::vcount;
        tmpvec.push_back(tmp1);
        auto e = new Expression(MoveRR,{tmp1},{r},{});
    }
    paramToCallWithCount = paramToCallWithCount > 0 ? paramToCallWithCount : 1;
    for(int i = paramToCallWithCount;i <= 7; i++)
    {
        int r =(int)(a0) + i;
        int tmp1 = ++Analyz::vcount;
        tmpvec.push_back(tmp1);
        auto e = new Expression(MoveRR,{tmp1},{r},{});
    } 
    //t0-t6调用者保存的寄存器会被def
    auto e = new Expression(Call,{(int)(a0),(int)(a1),(int)(a2),(int)(a3),(int)(a4),(int)(a5),(int)(a6),(int)(a7),
    (int)(t0),(int)(t1),(int)(t2),(int)(t3),(int)(t4),(int)(t5),(int)(t6)}
    ,paramvec,{},f);
    auto e1 = new Expression(MoveRR,{v},{(int)(a0)},{});
    for(int i = 0; i<=6; i++)
    {
        int r = (int)(t0) + i;
        auto e = new Expression(MoveRR,{r},{tmpvec[i]},{});
    } 
    for(int i = paramToCallWithCount;i <= 7; i++)
    {
        int r =(int)(a0) + i;
        auto e = new Expression(MoveRR,{r},{tmpvec[7 - paramToCallWithCount + i]},{});
    } 
    paramToCallWithCount = 0;
    }
    else
    {
        auto e = new Expression(Call,{(int)(a0)},{(int)(a0)},{},f);
        auto e1 = new Expression(MoveRR,{v},{(int)(a0)},{});
        paramToCallWithCount = 0;
    }
}
void Func::livelyAnalyz()
{
    
    for(auto e:exprs)
    {
        e->in = vector<int>();
        e->out = vector<int>();
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
            while(p1!=e->out.end() && p2!=e->def.end())   //计算out-def
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
            p1 = e->use.begin();                    //计算in = use+(out-def)
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
            //由于in是归并得到的,所以已经排好序
            e->out.clear();

            
            for(Expression* expr: e->nexts)        //计算out=nexts的in的并集
            {
                e->out.insert(e->out.end(),expr->in.begin(),expr->in.end());
            }
            std::sort(e->out.begin(),e->out.end());
            e->out.erase(std::unique(e->out.begin(),e->out.end()),e->out.end());//这句话可能是n方的
            
            //重写,避免使用unique,不过好像没什么区别,还是很慢呢
            /* vector<int> merge_counter;
            vector<bool> merge_finish;
            vector<int> merge_total;
            for(Expression* expr: e->nexts) 
            {
                merge_counter.push_back(0);
                if(expr->in.size()==0)
                    merge_finish.push_back(true);
                else  merge_finish.push_back(false);
                merge_total.push_back(expr->in.size());
            }
            while(1)
            {
                int min = (1<<30)-1;
                list<int> minarg;
                int i = 0;
                auto iter = e->nexts.begin();
                for(;i<merge_counter.size(); i++,iter++)
                if(merge_finish[i]==false)
                {
                    if((*iter)->in[merge_counter[i]] < min)
                    {
                        min = (*iter)->in[merge_counter[i]];
                        minarg.clear();
                        minarg.push_back(i);
                    }
                    else if((*iter)->in[merge_counter[i]] == min)
                    {
                        minarg.push_back(i);
                    }
                }
                if(minarg.size()==0) break;
                e->out.push_back(min);
                for(auto x:minarg)
                {
                    merge_counter[x]++;
                    if(merge_counter[x] == merge_total[x]) merge_finish[x] = true;
                }
            }
            */
            if(e->in != e->in1 || e->out != e->out1)
            {
                flag = false;
            } 
        }
        if(flag) break;
    }while(1);
}
void Func::DebugPrint()
{
    int DebugCounter =  0;
    cerr<<name<<endl;
    for(auto e: exprs)
    {
        cerr<<DebugCounter++<<"_";
        switch(e->type)
        {
    case Invalid:cerr<<"Invalid"<<endl;break;
    case MoveRI:cerr<<"MoveRI"<<endl;break;
    case MoveRR:cerr<<"MoveRR"<<endl;break;
    case ArithRR:cerr<<"ArithRR"<<endl;break;
    case ArithRRSame:cerr<<"ArithRRSame"<<endl;break;
    case ArithRI:cerr<<"ArithRI"<<endl;break;
    case Negative:cerr<<"Negative"<<endl;break;
    case ArrayWrite:cerr<<"ArrayWrite"<<endl;break;
    case ArrayRead:cerr<<"ArrayRead"<<endl;break;
    case IfRR:cerr<<"IfRR"<<endl;break;
    case IfRI:cerr<<"IfRI"<<endl;break;
    case IfIR:cerr<<"IfIR"<<endl;break;
    case Goto:cerr<<"Goto"<<endl;break;
    case Return:cerr<<"Return"<<endl;break;
    case ParamR:cerr<<"ParamR"<<endl;break;
    case ParamI:cerr<<"ParamI"<<endl;break;
    case Empty:cerr<<"Empty"<<endl;break;
    case Call:cerr<<"Call"<<endl;break;
    case Begin:cerr<<"Begin"<<endl;break;
    case FrameStore:cerr<<"FrameStore"<<endl;break;
    case FrameLoad:cerr<<"FrameLoad"<<endl;break;
    case FrameLoadAddr:cerr<<"FrameLoadAddr"<<endl;break;
    case GlobalLoad:cerr<<"GlobalLoad"<<endl;break;
    case GlobalLoadAddr:cerr<<"GlobalLoadAddr"<<endl;break;
    case Label:cerr<<"Label"<<endl;break;
        }
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
void Func::InitializeVectorSpace()
{
     //initialize Vectors
    spilledVariableFrameMap = vector<int>(Analyz::vcount + 1);
    initial = list<int>();
    simplifyWorklist = list<int>();
    freezeWorklist = list<int>();
    spillWorklist = list<int>();
    spilledNodes = list<int>();
    coalescedNodes = list<int>();
    coloredNodes = list<int>();
    selectStack = list<int>();
    coalescedMoves = list<Expression*>();
    constrainedMoves = list<Expression*>();
    frozenMoves  = list<Expression*>();
    worklistMoves = list<Expression*>();
    activeMoves  = list<Expression*>();

    adjMatrix = vector<vector<int>>(Analyz::vcount + 1);
    adjList = vector<list<int>>(Analyz::vcount + 1);
    degrees = vector<int>(Analyz::vcount+1);
    alias = vector<int>(Analyz::vcount+1);
    color = vector<int>(Analyz::vcount+1);
    status = vector<NodeStatus>(Analyz::vcount +1);
    useList = vector<list<Expression*>>(Analyz::vcount+1);
    defList = vector<list<Expression*>>(Analyz::vcount+1);
    moveList = vector<list<Expression*>>(Analyz::vcount + 1);
    for(int i = 0; i<= Analyz::vcount; i++)
    {
        adjMatrix[i] = vector<int>(Analyz::vcount+1);
    }

    //为保留节点预着色
    for(int i= 0; i<27; i++)
    {
        status[i] = Precolored;
        color[i] = i;
    }
}
void Func::InitColorAlgorithm()
{
    vector<int> tmp(Analyz::vcount+10);
    for(auto e:exprs)
    {
        for(auto v:e->use)
        {
            tmp[v] = 1;
            useList[v].push_back(e);
        }
        for(auto v:e->def)
        {
            //是否可做优化?
            defList[v].push_back(e);
        }
    }
    for(int v = 0; v<= Analyz::vcount; v++)
    {
        if(tmp[v] && status.at(v) != Precolored)
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
        else
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
            status[v] = Related;
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
    if(x!=y)
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
    // cerr<<"Simplify_"<<n<<endl;
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
        if(status[m]==Hard)
        {
            spillWorklist.remove(m);
            if(MoveRelated(m))
            {
                freezeWorklist.push_back(m);
                status[m] = Related;
            }
            else
            {
                simplifyWorklist.push_back(m);
                status[m] = Simple;
            }
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
    if(status[x] == Precolored && status[y] == Precolored)
    {
        frozenMoves.push_back(e);
        return;
    }
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
    // cerr<<"Coalesce_"<<u<<"_"<<v<<endl;
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
    auto tmp1 = Adjacent(u);
    auto tmp2 = Adjacent(u);
    std::set<int> st;
    for(auto x: tmp1)
    {
        if(degrees[x] >= colorNumber && st.count(x)==0) 
        {k++;st.insert(x);}
    }for(auto x: tmp2)
    {
        if(degrees[x] >= colorNumber && st.count(x)==0) 
        {k++;st.insert(x);}
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
    // cerr<<"Combine_"<<u<<" "<<v<<endl;
    if(status[v] == Related)
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
    moveList[u].sort();
    moveList[u].erase(std::unique(moveList[u].begin(),moveList[u].end()),moveList[u].end());
    EnableMoves(v);
    for(auto t:Adjacent(v))
    {
        AddEdge(t,u);
        DecrementDegree(t);
    }
    if(degrees[u] >= colorNumber && status[u] ==Related)
    {
        freezeWorklist.remove(u);
        spillWorklist.push_back(u);
        status[u] = Hard;
    }
}
void Func::FreezeAction()
{
    int u = freezeWorklist.front();
    // cerr<<"Freeze_"<<u<<endl;
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
        if(NodeMoves(v).size()==0 && degrees[v] < colorNumber && status[v] == Related) /*--*/
        {
            freezeWorklist.remove(v);
            simplifyWorklist.push_back(v);
            status[v] = Simple;
        } 
    }
}
void Func::SelectSpill()
{
    //选择一个启发式算法找出下一个溢出的节点,此处暂时选择度数最大的顶点
    /*int m = spillWorklist.back();
    spillWorklist.pop_back();*/

     int maxde = 0;int m = 0;
    for(auto x:spillWorklist)
    {
        if(degrees[x]> maxde)
        {
            maxde = degrees[x];
            m = x;
        }
    } 

    /* spillWorklist.sort();
    int  m =spillWorklist.front(); 
     */
    spillWorklist.remove(m);
    // cerr<<"Spill_"<<m<<endl;
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
        for(;i!=colorNumber && colorsToUse[i] == 1;i++);
        if(i == colorNumber)
        {
            spilledNodes.push_back(n);
            status[n] =Spilled;
        }
        else
        {
            coloredNodes.push_back(n);
            color[n] = i;
            status[n] = Colored;
        }
    }
    for(auto n: coalescedNodes)
    {
        color[n] = color[GetAlias(n)];
    }
}
void Func::RewriteProgram()
{
    for(auto v:spilledNodes)
    {
        //cerr<<"SPILL_"<<v<<endl;
        //暂时默认指针类型也是4字节..
        //向栈帧中添加
        int tmp = insert();
        spilledVariableFrameMap[v] = tmp;
        //生成新的命令和新的临时变量,
        //注:如果变量同时被读写,则添加两个临时变量,在图染色的时候他们会自动合并
        for(auto e:useList[v])
        {
            InsertExprForRead(e,v);
        }
        for(auto e:defList[v])
        {
            InsertExprForWrite(e,v);
        }
    }
}
void Func::InsertExprForRead(Expression* e,int v)
{
    int tempVar = GenTempVariable();
    int position = offset[spilledVariableFrameMap[v]] / 4;
    Expression * readExpr = new Expression(FrameLoad,{tempVar},{},{position},"","",false);
    auto it = find(exprs.begin(),exprs.end(),e);
    exprs.insert(it,readExpr);
    for(auto iter = e->right.begin(); iter!=e->right.end(); iter++)
    {
        if(*iter == v) 
        {
            *iter = tempVar;
        }
    }
    /* for(auto p: e->prevs)
    {
        p->nexts.remove(p);
        p->nexts.push_back(readExpr);
    }
    readExpr->prevs = e->prevs;
    readExpr->nexts.push_back(e); */
}
void Func::InsertExprForWrite(Expression* e, int v)
{
    int tempVar = GenTempVariable();
    int position = offset[spilledVariableFrameMap[v]] / 4;
    Expression * writeExpr = new Expression(FrameStore,{},{tempVar},{position},"","",false);
    auto it = find(exprs.begin(),exprs.end(),e);
    exprs.insert(++it,writeExpr);

    for(auto iter = e->left.begin(); iter!=e->left.end(); iter++)
    {
        if(*iter == v) 
        {
            *iter = tempVar;
        }
    }
    /* for(auto p: e->nexts)
    {
        p->prevs.remove(p);
        p->prevs.push_back(writeExpr);
    }
    writeExpr->nexts = e->nexts;
    writeExpr->prevs.push_back(e); */
}
int Func::GenTempVariable()
{
    return ++Analyz::vcount;
}
void Func::ColorAlgorithmMain()
{
    // DebugPrint();
    genFlow();
    // cerr<<"GenFlowFinish"<<endl;
    livelyAnalyz();
    // cerr<<"LivelyAnalyzFinish"<<endl;
    // DebugPrint();
    InitializeVectorSpace();
    InitColorAlgorithm();
    while(1)
    {
        if(!simplifyWorklist.empty())   Simplify();
        else if(!worklistMoves.empty()) Coalesce();
        else if(!freezeWorklist.empty()) FreezeAction();
        else if(!spillWorklist.empty()) SelectSpill();
        else break;
    }
    AssignColors();
    if(!spilledNodes.empty()) 
    {
        RewriteProgram();
        //GenCode();//Debug
        ColorAlgorithmMain(); //尾递归
    }
}
string Func::opstring(int op)
{
    char s[2];
    if(op=='e') return "==";
    else if(op=='n') return "!=";
    else 
    {
        s[0] = op;
        s[1] = 0;
        return s;
    }
}
string Func::opinstruct(int op)
{
    switch(op)
    {
        case '+': return "addw";
        case '-': return "subw";
        case '*': return "mulw";
        case '/': return "divw";
        case '%': return "remw";
        case '<': return "blt";
        case '>': return "bgt";
        case 'n': return "bne";
        case 'e': return "beq";
        default: return "OP_ERROR";
    }
}
void Func::OutputArithRIMul(int reg1,int reg2,int imm)
{
    //暂时只处理imm ==4 
    if(imm == 4)
    {
        cout<<"slli\t"<<REGNAMEFORVAR(reg1)<<","<<REGNAMEFORVAR(reg2)<<","<<2<<endl;
    }
    else
    {
        cout<<"ArithRIERROR!"<<endl;
    }
}
void Func::GenCode()
{
    cout<<name<<" ["<<paramCount<<"] ["<<frameSize/4<<"]"<<endl;
    for(auto e:exprs)
    {
        switch(e->type)
        {
            case ArithRR:cout<<REGNAMEFORVAR(e->left[0])<<" = "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[0])
                            <<" "<<REGNAMEFORVAR(e->right[1])<<endl;break;
            case ArithRRSame:cout<<REGNAMEFORVAR(e->left[0])<<" = "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[0])
                            <<" "<<REGNAMEFORVAR(e->right[0])<<endl;break;
            case ArithRI:cout<<REGNAMEFORVAR(e->left[0])<<" = "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[1])
                            <<" "<<e->imm[0]<<endl;break;
            case Negative:cout<<REGNAMEFORVAR(e->left[0])<<" = - "<<REGNAMEFORVAR(e->right[0]);break;
            case MoveRI: cout<<REGNAMEFORVAR(e->left[0])<<" = "<<e->imm[0]<<endl;break;
            case MoveRR: if(REGNAMEFORVAR(e->left[0])!=REGNAMEFORVAR(e->right[0]))
            {
                cout<<REGNAMEFORVAR(e->left[0])<<" = "<<REGNAMEFORVAR(e->right[0])<<endl;
            }
            break;
            case ArrayWrite: cout<<REGNAMEFORVAR(e->right[0])<<" ["<<e->imm[0]<<"] = "<<REGNAMEFORVAR(e->right[1])<<endl;break;
            case ArrayRead:  cout<<REGNAMEFORVAR(e->left[0])<<" = "<<REGNAMEFORVAR(e->right[0])<<" ["<<e->imm[0]<<"]"<<endl;break;
            case IfRR: cout<<"if "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[0])<<" "<<REGNAMEFORVAR(e->right[1])
                    <<" goto l"<<e->imm[1]<<endl;break;
            case IfRI: cout<<"if "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[0])<<" "<<"x0"
                    <<" goto l"<<e->imm[1]<<endl;break;
            case IfIR: cout<<"if "<<"x0"<<" "<<opstring(e->imm[0])<<" "<<REGNAMEFORVAR(e->right[0])
                    <<" goto l"<<e->imm[1]<<endl;break;
            case Goto: cout<<"goto l"<<e->imm[0]<<endl;break;
            case FrameLoad: cout<<"load "<<e->imm[0]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            case FrameStore: cout<<"store "<<REGNAMEFORVAR(e->right[0])<<" "<<e->imm[0]<<endl;break;
            case GlobalLoad: cout<<"load v"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            case GlobalLoadAddr: cout<<"loadaddr v"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            case FrameLoadAddr: cout<<"loadaddr "<<e->imm[0]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            //参数,函数

            case Empty:break;
            case Call: cout<<"call "<<e->funtocall<<endl;break;
            case Return : cout<<"return"<<endl;break;
            case Label:cout<<"l"<<e->imm[0]<<":"<<endl;break;
            case Invalid: cout<<"Invalid"<<endl;break;
            case Begin:break;
            default: cerr<<"TYPE_ERROR"<<(int)(e->type)<<endl;
        }
    }
    cout<<"end "<<name<<endl;
}
void Func::GenRiscv64()
{
    if(name=="f_main") name = "main";
    int stk = (frameSize/16 +1) *16;
    cout<<"\t.align\t1"<<endl;
    cout<<"\t.global\t"<<name<<endl;
    cout<<"\t.type\t"<<name<<", @function"<<endl;
    cout<<name<<":"<<endl;
    cout<<"\tadd\tsp,sp,"<<-stk<<endl;
    cout<<"\tsd\tra,"<<stk-8<<"(sp)"<<endl;
    for(auto e:exprs)
    {
        switch(e->type)
        {
            case ArithRR:cout<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[1])<<endl;break;
            case ArithRRD:cout<<"add\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[1])<<endl;break;
            case ArithRRSame:cout<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[0])<<endl;break;
            case ArithRI:if(e->imm[1]=='+')
            {
                cout<<"addiw\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","<<e->imm[0]<<endl;
            }
            else OutputArithRIMul(e->left[0],e->right[0],e->imm[0]);//parser需要修改
            break;
            case Negative:cout<<"subw\t"<<REGNAMEFORVAR(e->left[0])<<",zero,"<<REGNAMEFORVAR(e->right[0])<<endl;break;
            case MoveRI: cout<<"addiw\t"<<REGNAMEFORVAR(e->left[0])<<",zero,"<<e->imm[0]<<endl;break;
            case MoveRR: if(REGNAMEFORVAR(e->left[0])!=REGNAMEFORVAR(e->right[0]))
            {
                cout<<"mv\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0]);
            }
            break;
            case ArrayWrite: cout<<"sw\t"<<REGNAMEFORVAR(e->right[1])<<","<<e->imm[0]<<"("<<REGNAMEFORVAR(e->right[0])<<")"<<endl;break;
            case ArrayRead: cout<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<","<<e->imm[0]<<"("<<REGNAMEFORVAR(e->right[0])<<")"<<endl;break;
            case IfRR: cout<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->right[0])<<","<<REGNAMEFORVAR(e->right[1])<<",.L"<<e->imm[1]<<endl;break;
            case IfRI: cout<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->right[0])<<","<<"zero"<<",.L"<<e->imm[1]<<endl;break;
            case IfIR: cout<<opinstruct(e->imm[0])<<"\t"<<"zero"<<","<<REGNAMEFORVAR(e->right[0])<<",.L"<<e->imm[1]<<endl;break;
            case Goto: cout<<"j\t"<<".L"<<e->imm[0]<<endl;break;
            case FrameLoad: cout<<"sw\t"<<REGNAMEFORVAR(e->left[0])<<","<<4*e->imm[0]<<"(sp)"<<endl;break;
            case FrameStore: cout<<"lw\t"<<REGNAMEFORVAR(e->right[0])<<","<<4*e->imm[0]<<"(sp)"<<endl;break;
            case GlobalLoad: cout<<"lui\t"<<REGNAMEFORVAR(e->left[0])<<",%hi(v"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        cout<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<",%lo(v"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<")("<<REGNAMEFORVAR(e->left[0])<<")"<<endl;
                        break;
            case GlobalLoadAddr: cout<<"lui\t"<<REGNAMEFORVAR(e->left[0])<<",%hi(v"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        cout<<"addi\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->left[0])<<",%lo(v"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        break;
            case FrameLoadAddr: cout<<"addi\t"<<REGNAMEFORVAR(e->left[0])<<",sp,"<<4*e->imm[0]<<endl;break;
            case Empty: break;
            case Call: cout<<"call\t"<<e->funtocall<<endl;break;
            case Return: cout<<"ld\tra,"<<stk-8<<"(sp)"<<endl;
                    cout<<"addi\tsp,sp,"<<stk<<endl;
                    cout<<"jr\tra"<<endl;
                    break;
            case Label:cout<<".L"<<e->imm[0]<<":"<<endl;break;
            case Invalid:cout<<"INVALIDINSTRUCT"<<endl;break;
            case Begin:break;
            default: cerr<<"TYPE_ERROR"<<endl;
        }
    }
    cout<<"\t.size\t"<<name<<", .-"<<name<<endl;
}
void Func::Processor()
{
    InitFunEnv();
    ColorAlgorithmMain();
    //DebugPrintColorResult();
    //AssignPhysicsRegs();
    //DebugPrintPhysicsResult();
    //DebugPrint();
    //GenCode();
    GenRiscv64();
}
void Func::DebugPrintColorResult()
{
    for(int i = 0; i<= Analyz::vcount;i++)    {
        cerr<<i<<":"<<color[i]<<endl;
    }
}
void Func::DebugPrintPhysicsResult()
{
    for(int i = 0; i<= Analyz::vcount;i++)    {
        cerr<<i<<":"<<Reg::names[color[i]]<<endl;
    }
    //顺便输出参数表
    for(int i = 0; i< paramTable.size(); i++)
    {
        cerr<<"PARAM_"<<i<<":Variable_"<<paramTable[i]<<endl;
    }
}
int Func::colorNumber = 27;
vector<string> Reg::names = {"a0","a1","a2","a3","a4","a5","a6","a7",
    "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
    "t0","t1","t2","t3","t4","t5","t6"};