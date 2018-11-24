#include "analyz.h"
#define REGNAMEFORVAR(x) (Reg::names[color[GetAlias((x))]])
/* Variable::Variable(int _val,bool _param,int _pid):
    val(_val),param(_param),pid(_pid){} */
Analyz::Analyz()
{
    globalSize = 0; globalVariableCount = 0;
    // offset = vector<int>();size=vector<int>();
    // FuncMap = map<string,Func*>();
    // labelTable = map<int,Expression*>();
    // globalVaribleMap = map<int,int>();
    // globalVaribleType = map<int,int>();
}
Expression::Expression(ExprType _type,std::initializer_list<int> _left,
        std::initializer_list<int> _right,
        std::initializer_list<int> _imm,string _funtocall,string _funin,
        bool push)
:type(_type),left(_left),right(_right),
imm(_imm),funtocall(_funtocall),funin(_funin)
{
    if(push==false) return;
    bool leftGlobal = false;
    bool rightGlobal = false;
    for(int l:left)
    {
        if(Analyz::Instance.globalVaribleMap.find(l) != Analyz::Instance.globalVaribleMap.end())
        {
            leftGlobal = true;
            cerr<<"LEFTGLOBAL_"<<l<<endl;
        }
    }
    for(auto iter = right.begin();iter!=right.end();iter++)
    {
        if(Analyz::Instance.globalVaribleMap.find(*iter) !=
         Analyz::Instance.globalVaribleMap.end())
        {
            cerr<<"RIGHTGLOBAL_"<<*iter<<endl;
            //先load一下吧,有些语句可能不用load,以后再说.
            int tmp = Analyz::Instance.currentFunc().GenTempVariable();
            Expression* e1 = new Expression(GlobalLoad,{tmp},{},{*iter});
            *iter = tmp;
            rightGlobal = true;
        }
    }
    if(rightGlobal)
    {
        
    }
    if(leftGlobal)
    {
        int tmp1,tmp2;
        Expression* e1,* e2,* e3;
        switch(type)   //考虑到miniC转Eeyore时原生变量做左值只有赋值语句.
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
    if(_type == MoveRR)
    {
        this->isMove = true;
    }
    else
    {
        this->isMove = false;
    }
}
void Analyz::insert(int var,int s,int type)
{
    cerr<<globalVariableCount<<endl;
    offset.push_back(globalSize);
    size.push_back(s);
    globalSize += s;
    globalVaribleMap.insert(std::make_pair(var,globalVariableCount));
    globalVaribleType.insert(std::make_pair(var,type));
    globalVariableCount++;
}
int Func::insert(int s)
{
    int tmp = offset.size();
    offset.push_back(frameSize);
    size.push_back(s);
    frameSize += s;
    return tmp;
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
void Func::InitFunEnv()//此函数处理函数入口和出口出的寄存器管理,不处理call语句和return语句
{
    for(int i = 0; i<paramCount; i++)
    {
        int r = (int)(a0) + i;
        auto e= new Expression(MoveRR,{paramTable[i]},{r},{},"","",false);
        exprs.push_front(e);
    }
    //保存被调用者保存的寄存器
    for(int i = 0; i<= 11; i++)
    {
        int r = (int)(s0) + i;
        int tmp1 = ++Analyz::vcount;
        auto e =  new Expression(MoveRR,{tmp1},{r},{},"","");
        exprs.push_front(e);
    }
    //恢复被调用者保存的寄存器
    for(int i = 0; i<= 11; i++)
    {
        int r = (int)(s0) + i;
        int tmp1 = ++Analyz::vcount;
        auto e =  new Expression(MoveRR,{r},{tmp1},{},"","");
        exprs.push_back(e);
    }
}
void Analyz::process()
{
    for(auto f: funcs)
    {
        f.Processor();
    }
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
            e->out.erase(std::unique(e->out.begin(),e->out.end()),e->out.end());
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
    cerr<<name<<endl;
    for(auto e: exprs)
    {
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
    case ReturnR:cerr<<"ReturnR"<<endl;break;
    case ReturnI:cerr<<"ReturnI"<<endl;break;
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
    maxVariable = 0;
    for(auto e:exprs)
    {
        for(auto v:e->use)
        {
            if(v > maxVariable)
            {
                maxVariable = v;
            }
        }
        for(auto v:e->def)
        {
            if(v > maxVariable)
            {
                maxVariable = v;
            }
        }
    }
    spilledVariableFrameMap = vector<int>(maxVariable + 1);
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

    adjMatrix = vector<vector<int>>(maxVariable + 1);
    adjList = vector<list<int>>(maxVariable + 1);
    degrees = vector<int>(maxVariable+1);
    alias = vector<int>(maxVariable+1);
    color = vector<int>(maxVariable+1);
    status = vector<NodeStatus>(maxVariable +1);
    useList = vector<list<Expression*>>(maxVariable+1);
    defList = vector<list<Expression*>>(maxVariable+1);
    moveList = vector<list<Expression*>>(maxVariable + 1);
    for(int i = 0; i<= maxVariable; i++)
    {
        adjMatrix[i] = vector<int>(maxVariable+1);
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
    vector<int> tmp(maxVariable+10);
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
    cerr<<"Simplify_"<<n<<endl;
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
    cerr<<"Coalesce_"<<u<<"_"<<v<<endl;
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
    cerr<<"Combine_"<<u<<" "<<v<<endl;
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
    moveList[u].sort();
    moveList[u].erase(std::unique(moveList[u].begin(),moveList[u].end()),moveList[u].end());
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
    cerr<<"Freeze_"<<u<<endl;
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
    //选择一个启发式算法找出下一个溢出的节点,此处暂时选择编号最小的节点
    /*int m = spillWorklist.back();
    spillWorklist.pop_back();*/
    /* int minde = (1<<31) -1;int m = 0;
    for(auto x:spillWorklist)
    {
        if(degrees[x]< minde)
        {
            minde = degrees[x];
            m = x;
        }
    } */
    spillWorklist.sort();
    int  m =spillWorklist.front();
    spillWorklist.remove(m);
    cerr<<"Spill_"<<m<<endl;
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
        cerr<<"SPILL_"<<v<<endl;
        //暂时默认指针类型也是4字节..
        //向栈帧中添加
        int tmp = insert();
        spilledVariableFrameMap[v] = tmp;
        //生成新的命令和新的临时变量, 暂时不处理跳转指令
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
    ++maxVariable;
    return maxVariable;
}
void Func::ColorAlgorithmMain()
{
    genFlow();
    livelyAnalyz();
    DebugPrint();
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
/* void Func::AssignPhysicsRegs()
{
    PriorityRegs = {s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,t0,t1,t2,t3,t4,t5,t6,a0,a1,a2,a3,a4,a5,a6,a7};
    PhysicsColor = vector<int>(colorNumber,-1);
    ColorPhysics = vector<int>(colorNumber,-1);
    for(int i = 0 ; i< paramTable.size(); i++)
    {
        if(status[paramTable[i]] == Colored && ColorPhysics[paramReg(i)] == -1)
        {
            ColorPhysics[color[paramTable[i]]] = paramReg(i) ;
            PhysicsColor[paramReg(i)] = color[paramTable[i]];
        }
    }
    int j = 0;
    for(int i = 0; i<colorNumber; i++) 
    if(ColorPhysics[i] == -1)
    {
        while(PhysicsColor[j] != -1) j++;
        PhysicsColor[j] = i;
        ColorPhysics[i] = j;
    }
}
int Func::paramReg(int i)
{
    return i+19;
} */
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
void Func::GenCode()
{
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
            case IfRI: cout<<"if "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[0])<<" "<<e->imm[2]
                    <<" goto l"<<e->imm[1]<<endl;break;
            case IfIR: cout<<"if "<<e->imm[2]<<" "<<opstring(e->imm[0])<<" "<<REGNAMEFORVAR(e->right[0])
                    <<" goto l"<<e->imm[1]<<endl;break;
            case Goto: cout<<"goto l"<<e->imm[0]<<endl;break;
            case FrameLoad: cout<<"load "<<e->imm[0]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            case FrameStore: cout<<"store "<<REGNAMEFORVAR(e->right[0])<<" "<<e->imm[0]<<endl;break;
            case GlobalLoad: cout<<"load G"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            case GlobalLoadAddr: cout<<"loadaddr G"<<Analyz::Instance.globalVaribleMap[e->imm[0]]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;

            //参数,函数还未处理
            

            case Empty:break;
            case Label:cout<<"l"<<e->imm[0]<<":"<<endl;break;
            default: cerr<<"TYPE_ERROR"<<endl;
        }
    }
}
void Func::Processor()
{
    InitFunEnv();
    ColorAlgorithmMain();
    DebugPrintColorResult();
    //AssignPhysicsRegs();
    DebugPrintPhysicsResult();
    GenCode();
}
void Func::DebugPrintColorResult()
{
    for(int i = 0; i<= maxVariable;i++)    {
        cerr<<i<<":"<<color[i]<<endl;
    }
}
void Func::DebugPrintPhysicsResult()
{
    for(int i = 0; i<= maxVariable;i++)    {
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