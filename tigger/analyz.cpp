#include "analyz.h"
#define REGNAMEFORVAR(x) (Reg::names[color[GetAlias((x))]])

Analyz::Analyz()
{
    globalSize = 0; globalVariableCount = 0;
}
Expression::Expression(ExprType _type,vector<int> _left,
        vector<int> _right,
        vector<int> _imm,string _funtocall,string _funin,
        bool push)
:dead(false),isMove(false),visited(false),type(_type),left(_left),right(_right),
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
        if(AnalyzInstance.globalVaribleMap.find(l) != AnalyzInstance.globalVaribleMap.end())
        {
            leftGlobal = true;
        }
    }
    for(auto iter = right.begin();iter!=right.end();iter++)
    {
        if(AnalyzInstance.globalVaribleMap.find(*iter) !=
         AnalyzInstance.globalVaribleMap.end())
        {
            //先load一下吧,有些语句可能不用load,以后再说.
            int tmp = AnalyzInstance.currentFunc().GenTempVariable();
            if(AnalyzInstance.globalVaribleType[*iter] == 0)
            new Expression(GlobalLoad,{tmp},{},{*iter});
            else
            new Expression(GlobalLoadAddr,{tmp},{},{*iter});
            *iter = tmp;
            rightGlobal = true;
        }
        else if(AnalyzInstance.currentFunc().frameArrayTable.count(*iter))
        {
            int tmp = AnalyzInstance.currentFunc().GenTempVariable();
            new Expression(FrameLoadAddr,{tmp},{},
                {AnalyzInstance.currentFunc().offset[AnalyzInstance.currentFunc().frameArrayTable[*iter]]});
            *iter = tmp;
        }
    }
    if(leftGlobal)
    {
        int tmp1,tmp2;
        switch(type)   //考虑到miniC转Eeyore时全局变量做左值只有赋值语句.
        {
            case MoveRI:
            tmp1 = AnalyzInstance.currentFunc().GenTempVariable();
            new Expression(GlobalLoadAddr,{tmp1},{},{left[0]});
            tmp2 = AnalyzInstance.currentFunc().GenTempVariable();
            left[0] = tmp2;
            AnalyzInstance.currentFunc().exprs.push_back(this);
            new Expression(ArrayWrite,{},{tmp1,tmp2},{0});
            break;
            case MoveRR:
            tmp1 = AnalyzInstance.currentFunc().GenTempVariable();
            new Expression(GlobalLoadAddr,{tmp1},{},{left[0]});
            new Expression(ArrayWrite,{},{tmp1,right[0]},{0});
            break;
            default:
            cerr<<"LEFT_GLOBAL_TYPE_ERROR"<<endl; 
            break;
        }
    }
    else
    {
        AnalyzInstance.currentFunc().exprs.push_back(this);
    }
    
}
vector<int> globalType;
void Analyz::insert(int var,int s,int type)
{
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
void Analyz::GenGlobalTigger()
{
    for(int i = 0; i<globalVariableCount; i++)
    {
        if(globalType[i])
        {
            cout<<"v"<<i<<"= malloc "<<size[i]<<endl;
        }
        else
        {
            cout<<"v"<<i<<"= 0"<<endl;
        }
    }
}
int Func::insert(int s,int v)
{
    int tmp = offset.size();
    offset.push_back(frameSize);
    size.push_back(s);
    frameSize += s;
    if(frameSize > frameMaxSize) frameMaxSize = frameSize;
    frameArrayTable[v] = tmp;
    return tmp;
}
int Func::insert()
{
    int tmp = offset.size();
    offset.push_back(frameSize);
    size.push_back(4);
    frameSize += 4;
    if(frameSize > frameMaxSize) frameMaxSize = frameSize;
    return tmp;
}
void Func::frameFree()
{
    offset.pop_back();
    size.pop_back();
    frameSize -= 4;
}
Func::Func(int _paramCount,string _name):paramCount(_paramCount),paramToCallWithCount(0),frameSize(0),name(_name)
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
void Func::InitFunEnv()//此函数处理函数入口处的参数转移
{
    if(!calledStoredRegs.empty()) return ;
    for(int i = 0; i<paramCount; i++)
    {
        int r = (int)(a0) + i;
        auto e= new Expression(MoveRR,{paramTable[i]},{r},{},"","",false);
        auto it = exprs.begin();
        it++;
        exprs.insert(it,e);
    }
}
void Analyz::process()
{
    if(target == 0) GenGlobalTigger();
    for(auto f: funcs)
    {
        f.Processor();
    }
    if(target==1) GenGlobal();
    else if(target ==2) GenGlobal();
}
void Func::genFlow()
{
    for(auto iter = exprs.begin();iter!=exprs.end();++iter)
    {
        Expression* e = *iter;
        iter++;
        e->nexts.clear();
        e->prevs.clear();
        if(iter != exprs.end() && e->type != Goto && e->type != Return) //如果是Goto或Return则下一语句不是后继
        e->nexts.push_back(*iter);
        iter--;
        e->use = e->right;
        e->def = e->left;
        if(e->type==Goto)
        {
            e->nexts.push_back(AnalyzInstance.labelTable[e->imm[0]]);
        }
        else if(e->type==IfRR || e->type==IfRI || e->type==IfIR)
        {
            e->nexts.push_back(AnalyzInstance.labelTable[e->imm[1]]);
        }
        else if(e->type == Return)
        {
                e->nexts.push_back(exprs.back());
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
    /* if(calledStoredRegs.empty())
    {
        InitFunEnv();
    } */ //是否还有必要》？
    //传送返回值到a0
    if(t == 1)
    {
        new Expression(MoveRR,{(int)(a0)},{v},{});
    }
    else
    {
        new Expression(MoveRI,{(int)(a0)},{},{v});
    }
    new Expression(Return,{},{(int)(a0)},{});
}
void Func::CallParam(int v,int t)
{
    if(t == 1)
    {
    new Expression(MoveRR,{(int)(a0)+paramToCallWithCount},{v},{});
    }
    else
    {
    new Expression(MoveRI,{(int)(a0)+paramToCallWithCount},{},{v});
    }
    paramToCallWithCount++;
}
void Func::CallFunc(int v,string f)
{
    vector<int> paramvec;
    paramvec.push_back(int(a0));
    for(int i = 1; i<paramToCallWithCount;i ++)
    {
        paramvec.push_back((int)(a0) + i);
    }
    new Expression(Call,{(int)(a0)},paramvec,{},f);
    new Expression(MoveRR,{v},{(int)(a0)},{});
    paramToCallWithCount = 0;
}
void Func::SaveReg()
{
    //call语句
    for(auto it = exprs.begin(); it!= exprs.end(); it++)
    {
        if((*it)->type == Call)
        {
            int usedCount = 0;
            int usedRegs[30]={}; //既入口活跃又出口活跃的,并且不是a0,并且不是被调用者保存的，才需要恢复
            for(auto x: (*it)->in)
            {
                usedRegs[color[GetAlias(x)]]++;
            }
            for(auto x: (*it)->out)
            {
                usedRegs[color[GetAlias(x)]]++;
            }
            usedRegs[int(a0)] = 0;
            for(int i = int(s0); i<=int(s11); i++) usedRegs[i] =0;
            for(int i = 0; i<28; i++) if(usedRegs[i] == 2)
            {
                usedCount++;
                int tmp = insert(); //向栈帧中申请空间
                int position = offset[tmp] / 4;
                Expression * writeExpr = new Expression(FrameStore,{},{i},{position},"","",false);
                it = exprs.insert(it,writeExpr);//保存
                it++;
                it++;
                Expression * readExpr = new Expression(FrameLoad,{i},{},{position},"","",false);
                it = exprs.insert(it,readExpr);//回复
                it --;
            }
            while(usedCount --)
            {
                frameFree();//释放栈空间
            }
        }
    }
    //进入函数体,保存所有被调用者保存的寄存器，如果为main函数就不用
    if(name != "main")
    {
        auto it = exprs.begin();
        for(int i = (int)s0; i<=(int)s11; i++)
        {
            int tmp = insert(); //向栈帧中申请空间
            int position = offset[tmp] / 4;
            frameSaveTable[i] = position;
            Expression * writeExpr = new Expression(FrameStore,{},{i},{position},"","",false);
            it = exprs.insert(it,writeExpr);//保存
            it++;
        }
        //处理返回语句
        for(it = exprs.begin(); it != exprs.end(); ++it)
        if((*it)->type == Return)
        {
            for(auto x:frameSaveTable)
            {
                Expression *readExpr = new Expression(FrameLoad,{x.first},{},{x.second},"","",false);
                it = exprs.insert(it,readExpr);
                it ++;
            }
        }
    }
}
void Func::livelyAnalyz()
{
    
    for(auto e:exprs)
    {
        e->in.clear();
        e->out.clear();
        std::sort(e->use.begin(),e->use.end());
        std::sort(e->def.begin(),e->def.end());
    }
    /*
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
            
            if(e->in != e->in1 || e->out != e->out1)
            {
                flag = false;
            } 
        }
        if(flag) break;
    }while(1);
    */
    queue<Expression*> qex;
    qex.push(exprs.back());
    while(!qex.empty()) // 宽搜,去除死代码
    {
        set<int> liveVar;
        auto e = qex.front();
        qex.pop();
        for (auto f : e->nexts)
        {
            for (auto x : f->in)
            {
                liveVar.insert(x);
            }
        }
        //设置out
        bool flagout = false, flagin = false;
        for (auto x : liveVar)
        {
            if (e->out.find(x) == e->out.end())
            {
                flagout = true;
                e->out.insert(x);
            }
        }
        for (auto x : e->def)
        {
            auto it = liveVar.find(x);

            if (it != liveVar.end())
            {
                it = liveVar.erase(it);
            }
        }
        //继续;
        for (auto x : e->use)
        {
            liveVar.insert(x);
        }
        //加入in
        for (auto x : liveVar)
        {
            if (e->in.find(x) == e->in.end())
            {
                flagin = true;
                e->in.insert(x);
            }
        }
        if ((flagin || flagout || !e->visited || e->use.empty()) && e->dead == false)
        {
            e->visited = true;
            //加入前驱
            for (auto ee : e->prevs)
                if (ee->dead == false)
                {
                    qex.push(ee);
                }
        }
    }
    // DebugPrint();
}
void Func::OptimizeFlow()
{
    //常数传播
    auto it = exprs.begin();
    it++;
    for(; it!=exprs.end(); it++)
    {
        Expression * e = *it;
        if(e->prevs.empty()) continue;
        map<int,int> mp=e->prevs.front()->constant;
        for(auto it = mp.begin(); it != mp.end();)
        {
            int x = it->first;
            bool flag = true;
            for(auto f: e->prevs)
            {
                if(f->constant.find(x) == f->constant.end() ||
                (f->constant.find(x) != f->constant.end() && f->constant[x] != mp[x]))
                {
                    flag = false;
                    break;
                }
            }
            if(flag == false)
            {
                it = mp.erase(it);
            }
            else
            {
                ++it;
            }
        }
        e->constant = mp;
        if(e->def.empty()) continue;
        if(e->type == MoveRI)
        {
            e->constant[e->def[0]] = e->imm[0];
        }
        else
        {
            if(e->type != Call && e->type!= Return && e->type != Goto  && e->type != GlobalLoad && e->type!=GlobalLoadAddr 
            && e->type != FrameLoad && e->type != FrameLoadAddr && e->type != FrameStore && e->type != ArrayRead && e->type!= ArrayWrite)
            {
                bool flag = true;
                for(auto x: e->use)
                {
                    if(!e->constant.count(x))
                    {
                        flag = false;
                        break;
                    }
                }
                if(flag)
                {
                    int result;
                    switch(e->type)
                    {
                        case MoveRR: result = e->constant[e->use[0]] ;break;
                        case ArithRRD:
                        case ArithRR: result = calcarith(e->constant[e->use[0]],e->imm[0],e->constant[e->use[1]]);break;
                        case ArithRRSame: result = calcarith(e->constant[e->use[0]],e->imm[0],e->constant[e->use[0]]);break;
                        case ArithRI: result = calcarith(e->constant[e->use[0]],e->imm[1],e->imm[0]);break;
                        case Negative: result = calcarith(0,'-',e->constant[e->use[0]]);break;
                        case IfRR: result = calcarith(e->constant[e->use[0]],e->imm[0],e->constant[e->use[1]]);break;
                        case IfRI: result = calcarith(e->constant[e->use[0]],e->imm[0],e->imm[2]);break;
                        case IfIR: result = calcarith(e->imm[2],e->imm[0],e->constant[e->use[0]]);break;
                        default:cerr<<"CONSTANT ERROR"<<endl;break;
                    }
                    if(e->type == IfRR || e->type ==IfRI || e->type ==IfIR)
                    {
                        if(result) 
                        {
                            e->type = Goto;
                            e->left = e->def = {};
                            e->right = e->use = {};
                            e->imm = {e->imm[1]};
                        }
                        else
                        {
                            e->type = Empty; //设为空语句
                            e->left = e->def ={};
                            e->right = e->use = {};
                            e->imm = {};
                        }
                    }
                    else
                    {
                        e->type = MoveRI;
                        e->right = e->use = {};
                        e->imm = {result};
                        e->constant[e->def[0]] = result;
                    }
                }
                else
                {
                    for(auto x:e->def)
                    {
                        auto it = e->constant.find(x);
                        if(it!=e->constant.end())
                        {
                            e->constant.erase(it);
                        }
                    }
                }
            }
        }
    }
}

void Func::OptimizeDead()
{
    //消除死代码
    // DebugPrint();
    auto it= exprs.end();
    it--;
    for(; it != exprs.begin(); it --)
    {
        Expression * e = *it;
        bool flag = true;
        if(e->def.empty()) flag = false;
        else
        {
            for (auto x : e->def)
            {
                for (auto f : e->nexts)
                {
                    if (f->in.find(x) != f->in.end())
                    {
                        flag = false;
                        break;
                    }
                }
                if (!flag)
                    break;
            }
        }
        if(flag) e->dead = true;
        if(e->type == Call) e->dead = false; //Call不可能为死代码，除非运行不到
        if(e->visited == false) e->dead = true; //不可运行到的代码也是死代码
        if(e->dead)
        {
            it = exprs.erase(it);
            for(auto f: e->nexts)
            {
                f->prevs.remove(e);
                for(auto g: e->prevs)
                {
                    f->prevs.push_back(g);
                }
            }
            for(auto f: e->prevs)
            {
                f->nexts.remove(e);
                for(auto g: e->nexts)
                {
                    f->nexts.push_back(g);
                }
            }
        }
    }

}
void Func::DebugPrint()
{
    int DebugCounter =  0;
    cerr<<name<<endl;
    for(auto e: exprs)
    {
        cerr<<DebugCounter++<<"_";
        cerr<<e->dead<<" ";
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
    default: cerr<<"DEBUG_PRINT_TYPEERROR"<<endl;
        }
        cerr<<"def :";DebugPrint(e->def);
        cerr<<"use :";DebugPrint(e->use);
        cerr<<"in :";DebugPrint(e->in);
        cerr<<"out :";DebugPrint(e->out);
    }
    cerr<<"---------------"<<endl;
}
template <class T>
void Func::DebugPrint(T  & v)
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
            int dd = *(e->def.begin());
            int uu = *(e->use.begin());
            moveList[dd].push_back(e);
            moveList[uu].push_back(e);
            worklistMoves.push_back(e);
            for(auto i = e->in.begin();i!=e->in.end(); i++)
            {
                auto j = ++i;
                i--;
                for(; j!= e->in.end(); j++)
                {
                    if((*i == dd && *j== uu) || (*i==uu && *j == dd))
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
        {auto j = ++i;
                i--;
                for(; j!= e->in.end(); j++)
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

    spillWorklist.remove(m);
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
}
void Func::OptimizeLoadStore()
{
    map<int,int> frmReg;
    map<int,int> frmSts;//0 无效 1 有效寄存器Load 2 有效寄存器LoadAddr
    map<int,Expression *> lastLoad;
    map<int,Expression *> lastStore;
    int regs[28] = {};
    frmReg.clear(); frmSts.clear();memset(regs,-1,sizeof(regs));
    for(auto it = exprs.begin(); it != exprs.end(); it++)
    {
        Expression * e = *it;
        if(e->type == Goto || e->type == IfRR || e->type == IfRI || e->type == IfIR 
                || e->type == Call ||  e->type == Return || e->type == Label || 
                e->type == ArrayWrite)
        {
            frmReg.clear(); frmSts.clear(); memset(regs,-1,sizeof(regs));
        }
        else if(e->type == FrameLoad)
        {
            if(frmSts[e->imm[0]] == 1 &&color[GetAlias(e->left[0])] == frmReg[e->imm[0]] && regs[frmReg[e->imm[0]]] == e->imm[0])
            {
                e->dead = true;
            }
            else
            {
                if(regs[frmReg[e->imm[0]]] != -1)
                {
                    frmReg[e->imm[0]] = 0;
                    frmSts[e->imm[0]] = 0;
                }
                regs[frmReg[e->imm[0]]] = -1;
                regs[color[GetAlias(e->left[0])]] =e->imm[0];
                frmSts[e->imm[0]] = 1;
                frmReg[e->imm[0]] = color[GetAlias(e->left[0])];
            }
        }
        else if(e->type == FrameLoadAddr)
        {
            if(frmSts[e->imm[0]] == 2 &&color[GetAlias(e->left[0])] == frmReg[e->imm[0]] && regs[frmReg[e->imm[0]]] == e->imm[0])
            {
                e->dead = true;
            }
            else
            {
                if(regs[frmReg[e->imm[0]]] != -1)
                {
                    frmReg[e->imm[0]] = 0;
                    frmSts[e->imm[0]] = 0;
                }
                regs[frmReg[e->imm[0]]] = -1;
                regs[color[GetAlias(e->left[0])]] =e->imm[0];
                frmSts[e->imm[0]] = 2;
                frmReg[e->imm[0]] = color[GetAlias(e->left[0])];
            }
        }
        else
        {
            for(auto x: e->left)
            {
                if(regs[color[GetAlias(x)]] != -1)
                {
                    frmSts[regs[color[GetAlias(x)]]] = 0;
                    frmReg[regs[color[GetAlias(x)]]] = 0;
                    regs[color[GetAlias(x)]] = -1;
                }
            }
        }
    }
    
    //优化先Load再Store 中间没有使用的
    frmReg.clear(); frmSts.clear();lastLoad.clear();lastStore.clear(); memset(regs,-1,sizeof(regs));
    for(auto it = exprs.begin();it != exprs.end(); ++it)
    {
        Expression * e = *it;
        if(e->type == FrameLoad)
        {
            if(regs[frmReg[e->imm[0]]] != -1)
            {
                frmReg[regs[frmReg[e->imm[0]]]] = 0;
                frmSts[regs[frmReg[e->imm[0]]]] = 0;
                regs[frmReg[e->imm[0]]] = -1;
            }
            frmReg[e->imm[0]] = color[GetAlias(e->left[0])];
            frmSts[e->imm[0]] = 1;
            lastLoad[e->imm[0]] = e;
        }
        else if(e->type == FrameStore)
        {
            if(frmSts[e->imm[0]] == 1 && frmReg[e->imm[0]] == color[GetAlias(e->right[0])])
            {
                lastLoad[e->imm[0]] -> dead = true;
                e-> dead = true;
            }
        }
        else if(e->type == Goto || e->type == IfRR || e->type == IfRI || e->type == IfIR 
                || e->type == Call ||  e->type == Return || e->type == Label || 
                e->type == ArrayWrite)
        {
            frmReg.clear(); frmSts.clear(); lastLoad.clear();lastStore.clear(); memset(regs,-1,sizeof(regs));
        }
        else
        {
            for(auto x: e->left)
            {
                if(regs[color[GetAlias(x)]] != -1)
                {
                    frmSts[regs[color[GetAlias(x)]]] = 0;
                    frmReg[regs[color[GetAlias(x)]]] = 0;
                    regs[color[GetAlias(x)]] = -1;
                }
            }
            for(auto x: e->right)
            {
                if(regs[color[GetAlias(x)]] != -1)
                {
                    frmSts[regs[color[GetAlias(x)]]] = 0;
                    frmReg[regs[color[GetAlias(x)]]] = 0;
                    regs[color[GetAlias(x)]] = -1;
                }
            }
        }
    }
}
int Func::GenTempVariable()
{
    return ++Analyz::vcount;
}
void Func::ColorAlgorithmMain()
{
    genFlow();
    livelyAnalyz();
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
        cout<<"\tslli\t"<<REGNAMEFORVAR(reg1)<<","<<REGNAMEFORVAR(reg2)<<","<<2<<endl;
    }
    else
    {
        cout<<"ArithRIERROR!"<<endl;
    }
}
void Func::GenCode()
{
    cout<<name<<" ["<<paramCount<<"] ["<<frameMaxSize/4<<"]"<<endl;
    for(auto e:exprs)  if(!e->dead)
    {
        switch(e->type)
        {
            case ArithRR:
            case ArithRRD:
                cout<<REGNAMEFORVAR(e->left[0])<<" = "<<REGNAMEFORVAR(e->right[0])<<" "<<opstring(e->imm[0])
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
            case GlobalLoad: cout<<"load v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
            case GlobalLoadAddr: cout<<"loadaddr v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<" "<<REGNAMEFORVAR(e->left[0])<<endl;break;
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
    int stk = (frameMaxSize+8+15)/16 * 16;
    cout<<"\t.align\t1"<<endl;
    cout<<"\t.global\t"<<name<<endl;
    cout<<"\t.type\t"<<name<<", @function"<<endl;
    cout<<name<<":"<<endl;
    cout<<"\tadd\tsp,sp,"<<-stk<<endl;
    cout<<"\tsd\tra,"<<stk-8<<"(sp)"<<endl;
    for(auto e:exprs) if(!e->dead)
    {
        switch(e->type)
        {
            case ArithRR:cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[1])<<endl;break;
            case ArithRRD:cout<<"\t"<<"add\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[1])<<endl;break;   //ArithRRD只用于计算地址
            case ArithRRSame:cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[0])<<endl;break;
            case ArithRI:if(e->imm[1]=='+')
            {
                cout<<"\t"<<"addiw\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","<<e->imm[0]<<endl;
            }
            else OutputArithRIMul(e->left[0],e->right[0],e->imm[0]);//parser需要修改
            break;
            case Negative:cout<<"\t"<<"subw\t"<<REGNAMEFORVAR(e->left[0])<<",zero,"<<REGNAMEFORVAR(e->right[0])<<endl;break;
            case MoveRI: cout<<"\t"<<"addiw\t"<<REGNAMEFORVAR(e->left[0])<<",zero,"<<e->imm[0]<<endl;break;
            case MoveRR: if(REGNAMEFORVAR(e->left[0])!=REGNAMEFORVAR(e->right[0]))
            {
                cout<<"\t"<<"mv\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<endl;
            }
            break;
            case ArrayWrite: cout<<"\t"<<"sw\t"<<REGNAMEFORVAR(e->right[1])<<","<<e->imm[0]<<"("<<REGNAMEFORVAR(e->right[0])<<")"<<endl;break;
            case ArrayRead: cout<<"\t"<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<","<<e->imm[0]<<"("<<REGNAMEFORVAR(e->right[0])<<")"<<endl;break;
            case IfRR: cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->right[0])<<","<<REGNAMEFORVAR(e->right[1])<<",.L"<<e->imm[1]<<endl;break;
            case IfRI: cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->right[0])<<","<<"zero"<<",.L"<<e->imm[1]<<endl;break;
            case IfIR: cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<"zero"<<","<<REGNAMEFORVAR(e->right[0])<<",.L"<<e->imm[1]<<endl;break;
            case Goto: cout<<"\t"<<"j\t"<<".L"<<e->imm[0]<<endl;break;
            case FrameLoad: cout<<"\t"<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<","<<4*e->imm[0]<<"(sp)"<<endl;break;
            case FrameStore: cout<<"\t"<<"sw\t"<<REGNAMEFORVAR(e->right[0])<<","<<4*e->imm[0]<<"(sp)"<<endl;break;
            case GlobalLoad: cout<<"\t"<<"lui\t"<<REGNAMEFORVAR(e->left[0])<<",%hi(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        cout<<"\t"<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<",%lo(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")("<<REGNAMEFORVAR(e->left[0])<<")"<<endl;
                        break;
            case GlobalLoadAddr: cout<<"\t"<<"lui\t"<<REGNAMEFORVAR(e->left[0])<<",%hi(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        cout<<"\t"<<"addi\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->left[0])<<",%lo(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        break;
            case FrameLoadAddr: cout<<"\t"<<"addi\t"<<REGNAMEFORVAR(e->left[0])<<",sp,"<<4*e->imm[0]<<endl;break;
            case Empty: break;
            case Call: 
                    cout<<"\t"<<"call\t"<<e->funtocall<<endl;
                break;
            case Return: cout<<"\t"<<"ld\tra,"<<stk-8<<"(sp)"<<endl;
                    cout<<"\t"<<"addi\tsp,sp,"<<stk<<endl;
                    cout<<"\t"<<"jr\tra"<<endl;
                    break;
            case Label:cout<<"\t"<<".L"<<e->imm[0]<<":"<<endl;break;
            case Invalid:cout<<"\t"<<"INVALIDINSTRUCT"<<endl;break;
            case Begin:break;
            default: cerr<<"TYPE_ERROR"<<endl;
        }
    }
    cout<<"\t.size\t"<<name<<", .-"<<name<<endl;
}
void Func::GenRiscv32()
{
    if(name=="f_main") name = "main";
    int stk = (frameMaxSize+8+15)/16 * 16;
    cout<<"\t.align\t1"<<endl;
    cout<<"\t.global\t"<<name<<endl;
    cout<<"\t.type\t"<<name<<", @function"<<endl;
    cout<<name<<":"<<endl;
    cout<<"\tadd\tsp,sp,"<<-stk<<endl;
    cout<<"\tsd\tra,"<<stk-4<<"(sp)"<<endl;
    for(auto e:exprs) if(!e->dead)
    {
        switch(e->type)
        {
            case ArithRR:cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[1])<<endl;break;
            case ArithRRD:cout<<"\t"<<"add\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[1])<<endl;break;   //ArithRRD只用于计算地址
            case ArithRRSame:cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","
            <<REGNAMEFORVAR(e->right[0])<<endl;break;
            case ArithRI:if(e->imm[1]=='+')
            {
                cout<<"\t"<<"addiw\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<","<<e->imm[0]<<endl;
            }
            else OutputArithRIMul(e->left[0],e->right[0],e->imm[0]);//parser需要修改
            break;
            case Negative:cout<<"\t"<<"subw\t"<<REGNAMEFORVAR(e->left[0])<<",zero,"<<REGNAMEFORVAR(e->right[0])<<endl;break;
            case MoveRI: cout<<"\t"<<"addiw\t"<<REGNAMEFORVAR(e->left[0])<<",zero,"<<e->imm[0]<<endl;break;
            case MoveRR: if(REGNAMEFORVAR(e->left[0])!=REGNAMEFORVAR(e->right[0]))
            {
                cout<<"\t"<<"mv\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->right[0])<<endl;
            }
            break;
            case ArrayWrite: cout<<"\t"<<"sw\t"<<REGNAMEFORVAR(e->right[1])<<","<<e->imm[0]<<"("<<REGNAMEFORVAR(e->right[0])<<")"<<endl;break;
            case ArrayRead: cout<<"\t"<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<","<<e->imm[0]<<"("<<REGNAMEFORVAR(e->right[0])<<")"<<endl;break;
            case IfRR: cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->right[0])<<","<<REGNAMEFORVAR(e->right[1])<<",.L"<<e->imm[1]<<endl;break;
            case IfRI: cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<REGNAMEFORVAR(e->right[0])<<","<<"zero"<<",.L"<<e->imm[1]<<endl;break;
            case IfIR: cout<<"\t"<<opinstruct(e->imm[0])<<"\t"<<"zero"<<","<<REGNAMEFORVAR(e->right[0])<<",.L"<<e->imm[1]<<endl;break;
            case Goto: cout<<"\t"<<"j\t"<<".L"<<e->imm[0]<<endl;break;
            case FrameLoad: cout<<"\t"<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<","<<4*e->imm[0]<<"(sp)"<<endl;break;
            case FrameStore: cout<<"\t"<<"sw\t"<<REGNAMEFORVAR(e->right[0])<<","<<4*e->imm[0]<<"(sp)"<<endl;break;
            case GlobalLoad: cout<<"\t"<<"lui\t"<<REGNAMEFORVAR(e->left[0])<<",%hi(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        cout<<"\t"<<"lw\t"<<REGNAMEFORVAR(e->left[0])<<",%lo(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")("<<REGNAMEFORVAR(e->left[0])<<")"<<endl;
                        break;
            case GlobalLoadAddr: cout<<"\t"<<"lui\t"<<REGNAMEFORVAR(e->left[0])<<",%hi(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        cout<<"\t"<<"addi\t"<<REGNAMEFORVAR(e->left[0])<<","<<REGNAMEFORVAR(e->left[0])<<",%lo(v"<<AnalyzInstance.globalVaribleMap[e->imm[0]]<<")"<<endl;
                        break;
            case FrameLoadAddr: cout<<"\t"<<"addi\t"<<REGNAMEFORVAR(e->left[0])<<",sp,"<<4*e->imm[0]<<endl;break;
            case Empty: break;
            case Call: 
                    cout<<"\t"<<"call\t"<<e->funtocall<<endl;
                break;
            case Return: cout<<"\t"<<"ld\tra,"<<stk-4<<"(sp)"<<endl;
                    cout<<"\t"<<"addi\tsp,sp,"<<stk<<endl;
                    cout<<"\t"<<"jr\tra"<<endl;
                    break;
            case Label:cout<<"\t"<<".L"<<e->imm[0]<<":"<<endl;break;
            case Invalid:cout<<"\t"<<"INVALIDINSTRUCT"<<endl;break;
            case Begin:break;
            default: cerr<<"TYPE_ERROR"<<endl;
        }
    }
    cout<<"\t.size\t"<<name<<", .-"<<name<<endl;    
}
void Func::Processor()
{
    InitFunEnv();
    genFlow();
    checkReturn();
    genFlow();
    livelyAnalyz();
    OptimizeFlow();
    genFlow();
    livelyAnalyz();
    OptimizeDead();
    ColorAlgorithmMain();
    SaveReg();
    OptimizeLoadStore();
    if(target==0) GenCode();
    else if(target == 1) GenRiscv64();
    else if(target == 2) GenRiscv32();
}
void Func::checkReturn()
{
    bool flag = false;
    Expression * e = exprs.back();
    for(auto f: e->prevs)
    {
        if( f->type != Return)
        {
            flag = true;
            auto it = find(exprs.begin(),exprs.end(),f);
            it++;
            Expression * e0 = new Expression(MoveRI,{(int)(a0)},{},{0},"","",false); 
            Expression * e1 = new Expression(Return,{},{(int)(a0)},{},"","",false); //返回a0=0值
            it = exprs.insert(it,e0);
            it++;
            exprs.insert(it,e1);
        }
    }
    if(flag)
    {
        AnalyzInstance.ReportWarning("Control reaches end of non-void function "+name);
    }
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
    for(size_t i = 0; i< paramTable.size(); i++)
    {
        cerr<<"PARAM_"<<i<<":Variable_"<<paramTable[i]<<endl;
    }
}
void Analyz::ReportWarning(string s)
{
    cerr<<"Warning: "<<s<<endl;
}
int Func::colorNumber = 27;
vector<string> Reg::names = {"a0","a1","a2","a3","a4","a5","a6","a7",
    "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
    "t0","t1","t2","t3","t4","t5","t6"};