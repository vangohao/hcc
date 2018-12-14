#ifndef __ANALYZ_H
#define __ANALYZ_H
#include "tigger.h"
enum ExprType
{
    Invalid,
    MoveRI,
    MoveRR,
    ArithRR,
    ArithRRD,
    ArithRRSame,
    ArithRI,  //Allow + Only
    Negative,
    ArrayWrite,
    ArrayRead,
    IfRR,
    IfRI,
    IfIR,
    Goto,
    Return,
    ParamR,
    ParamI,
    Empty,
    Call,
    Begin,
    FrameStore,
    FrameLoad,
    FrameLoadAddr,
    GlobalLoad,
    GlobalLoadAddr,
    Label,
};
enum NodeStatus
{
    Simple,
    Hard,
    Spilled,
    Related,
    Colored,
    Stacked,
    Coalesced,
    Precolored,
};
enum PhysicsRegs
{
    a0,a1,a2,a3,a4,a5,a6,a7,                 //参数,不做参数的部分,是调用者保存还是? 暂时设为调用者保存
    s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,  //被调用者保存
    t0,t1,t2,t3,t4,t5,t6,                   //调用者保存
};
class Expression
{
public:
    ExprType type;
    bool isMove;
    bool dead;
    vector<int> left;
    vector<int> right;
    vector<int> imm;
    vector<int> use;
    vector<int> def;
    set<int> in;
    vector<int> in1;
    set<int> out;
    vector<int> out1;
    list<Expression*> nexts;
    list<Expression*> prevs;
    string funtocall;
    string funin;
    Expression(ExprType _type,vector<int> _left,
    vector<int> _right,vector<int> _imm,
    string _funtocall="",string _funin="",bool push=true);
};
class Reg
{
    public:
    static vector<string> names;
};
class Func
{
public:
    int paramCount;
    int paramToCallWithCount;
    vector<int> calledStoredRegs;
    int frameSize;
    unordered_map<int,int> frameSaveTable; 
    unordered_map<int,int> frameArrayTable;
    string name;
    vector<int> offset;
    vector<int> size;
    vector<int> paramTable;
    unordered_map<int,int> paramTableReverse;
    list<Expression*> exprs;
    vector<int> spilledVariableFrameMap; //由变量id映射到上面的offset和size数组的下标

    //进入函数环境处理
    void InitFunEnv();

    //Color Algorithm
    static int colorNumber;
    list<int> initial;
    list<int> simplifyWorklist;
    list<int> freezeWorklist;
    list<int> spillWorklist;          //高度数节点表
    list<int> spilledNodes;
    list<int> coalescedNodes;
    list<int> coloredNodes;
    list<int> selectStack;
    list<Expression*> coalescedMoves;
    list<Expression*> constrainedMoves;
    list<Expression*> frozenMoves;
    list<Expression*> worklistMoves;
    list<Expression*> activeMoves;
    vector<vector<int>> adjMatrix;
    vector<list<int>> adjList;
    vector<int> degrees;
    vector<int> alias;
    vector<int> color;
    vector<NodeStatus> status;
    vector<list<Expression*>> useList;
    vector<list<Expression*>> defList;
    vector<list<Expression*>> moveList;


    void ColorAlgorithmMain();
    void AddEdge(int x,int y);
    void livelyAnalyz();
    void InitColorAlgorithm();
    void DecrementDegree(int m);
    bool MoveRelated(int n);
    list<Expression*>& NodeMoves(int n);
    list<int>& Adjacent(int n);
    void Simplify();
    void Coalesce();
    int GetAlias(int x);
    void AddWorklist(int u);
    bool TestPrecoloredCombine(int u/*precolored*/,int v);
    bool TestConservative(int u,int v);
    void Combine(int u,int v);
    void EnableMoves(int m);
    void FreezeMoves(int u);
    void FreezeAction();
    void SelectSpill();
    void AssignColors();
    void RewriteProgram();
    void InsertExprForWrite(Expression* e,int v);
    void InsertExprForRead(Expression* e,int v);
    int GenTempVariable();

    int insert(int s,int v);
    int insert();
    Func(int _paramCount,string _name);
    void genFlow();
    void OptimizeFlow();
    void InitializeVectorSpace();
    void Processor();
    void SaveReg();

    //GenCode
    string opstring(int op);
    string opinstruct(int op);
    void OutputArithRIMul(int reg1,int reg2,int imm);
    void GenCode();
    void GenRiscv64();
    void ReturnFunc(int v,int t);
    void CallParam(int v,int t);
    void CallFunc(int v,string f);

    //Debug
    void DebugPrint();
    template<class T>
    void DebugPrint(T & v);
    void DebugPrintColorResult();
    void DebugPrintPhysicsResult();
private:
};
class Analyz
{
public:
    static int vcount;//eeyore中的变量编号从28开始,0-27为物理寄存器预留
    //static Analyz Instance;
    int globalSize;
    int globalVariableCount;
    vector<int> offset;
    vector<int> size;
    //vector<int> globalType;
    //固定0~26为预着色节点,在Analyz对象创建时建立
    unordered_map<string,Func*> FuncMap;
    unordered_map<int,Expression*> labelTable; 
    unordered_map<int,int> globalVaribleMap;
    unordered_map<int,int> globalVaribleType;
    void insert(int var,int s=4,int type = 0);
                //type0表示int,1表示数组
    vector<Func> funcs;
    Func& currentFunc();
    void process();
    //void GenPutGet();
    void GenGlobalTigger();
    void GenGlobal();
    Analyz();
};
extern int target;//0 for tigger; 1 for RiscV64
extern Analyz AnalyzInstance;
#endif
