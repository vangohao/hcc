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
enum NodeStatus
{
    Simple,
    Hard,
    Spilled,
    Freeze,
    Colored,
    Stacked,
    Coalesced,
    Precolored,
};
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

    //Color Algorithm
    static int colorNumber;
    list<int> precolored;
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
    vector<NodeStatus> status;
    vector<vector<Expression*>> moveList;
    vector<int> alias;
    vector<int> color;
    void AddEdge(int x,int y);
    void livelyAnalyz();
    void initColorAlgorithm();
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

    void insert(int s);
    Func(int _paramCount,string _name);
    void genFlow();
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