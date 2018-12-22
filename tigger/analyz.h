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
    ExprType type;          //类型
    bool isMove;            //是否为传送指令
    bool dead;              //是否为死代码
    bool visited;           //是否被访问过(用于活性分析)
    vector<int> left;       //左值变量
    vector<int> right;      //右值变量
    vector<int> imm;        //直接数
    vector<int> use;        //使用的变量集合
    vector<int> def;        //定义的变量集合
    set<int> in;            //入口活跃集合
    set<int> out;           //出口活跃集合
    list<Expression*> nexts;//下一条指令集合
    list<Expression*> prevs;//上一条指令集合
    map<int,int> constant;  //常数变量表,用于常数传播
    string funtocall;       //仅用于call语句
     //构造函数,用于创建Expression并自动将指针加入当前所在函数的exprs表
    Expression(ExprType _type,vector<int> _left,
    vector<int> _right,vector<int> _imm,
    string _funtocall="",bool push=true);
};
class Func
{
public:
    Func(int _paramCount,string _name); //构造函数
    void Processor();                   //主函数
    int insert(int s,int v);            //向栈空间添加变量,s为大小,v为变量id,返回值为栈上的编号
    void ReturnFunc(int v,int t);       //处理Return语句,v为变量id或常数值,t为选项(0表示常数,1表示变量)
    void CallParam(int v,int t);        //处理Param语句,v和t的含义同上
    void CallFunc(int v,string f);      //处理call语句,v为存返回值变量,f为call的函数名称
    int getParamVar(int r);             //获取形参对应的局部变量编号
    friend class Expression;            //将Expression声明为友元
private:
    int paramCount;                     //参数数量
    int paramToCallWithCount;           //调用参数计数器
    vector<int> calledStoredRegs; 
    int frameSize;                      //栈空间大小(不含保存调用者保存寄存器的临时空间)
    int frameMaxSize;                   //栈空间大小
    unordered_map<int,int> frameSaveTable;  //记录被调用者保存寄存器的保存位置
    unordered_map<int,int> frameArrayTable; //从栈上数组变量id映射到栈上位置
    string name;                        //函数名称
    vector<int> offset;                 //栈上数据位置
    vector<int> size;                   //栈上数据大小
    vector<int> paramTable;             //形参对应局部变量表
    unordered_map<int,int> paramTableReverse;
    list<Expression*> exprs;            //语句表
    vector<int> spilledVariableFrameMap; //由变量id映射到上面的offset和size数组的下标


    //Color Algorithm图染色算法
    static int colorNumber;             //颜色数
    list<int> initial;                  //待初始化的节点表
    list<int> simplifyWorklist;         //低度数传送无关节点表
    list<int> freezeWorklist;           //低度数传送相关节点表
    list<int> spillWorklist;            //高度数节点表
    list<int> spilledNodes;             //溢出节点
    list<int> coalescedNodes;           //已合并节点
    list<int> coloredNodes;             //已染色节点
    list<int> selectStack;              //栈
    list<Expression*> coalescedMoves;   //已合并传送指令
    list<Expression*> constrainedMoves; //已约束传送指令
    list<Expression*> frozenMoves;      //已冻结传送指令
    list<Expression*> worklistMoves;    //待合并的传送指令
    list<Expression*> activeMoves;      //活跃的传送指令
    vector<vector<int>> adjMatrix;      //邻接矩阵
    vector<list<int>> adjList;          //邻接表
    vector<int> degrees;                //度
    vector<int> alias;                  //别名
    vector<int> color;                  //颜色
    vector<NodeStatus> status;          //顶点状态
    vector<list<Expression*>> useList;  //使用该变量的指令表
    vector<list<Expression*>> defList;  //定义该变量的指令表
    vector<list<Expression*>> moveList; //与该变量有关的传送指令表


    void ColorAlgorithmMain();          //图染色主函数
    void AddEdge(int x,int y);          //建图添加边
    void livelyAnalyz();                //活性分析
    void InitColorAlgorithm();          //初始化
    void DecrementDegree(int m);        //顶点度数减1
    bool MoveRelated(int n);            //n是否是传送相关的
    list<Expression*>& NodeMoves(int n);//与n相关的传送指令(未冻结的)
    list<int>& Adjacent(int n);         //n的邻点集
    void Simplify();                    //简化
    void Coalesce();                    //合并
    int GetAlias(int x);                //获取别名(由合并产生)
    void AddWorklist(int u);            //加入工作表
    bool TestPrecoloredCombine(int u/*precolored*/,int v); //测试预着色节点相关传送指令是否可以合并
    bool TestConservative(int u,int v); //测试传送指令是否可以保守合并
    void Combine(int u,int v);          //合并
    void EnableMoves(int m);            //将m相关的传送指令设为待合并的
    void FreezeMoves(int u);            //将u相关的传送指令冻结
    void FreezeAction();                //冻结
    void SelectSpill();                 //选择高度数节点溢出
    void AssignColors();                //分配颜色
    void RewriteProgram();              //重写程序,对于有真实溢出的情况
    void InsertExprForWrite(Expression* e,int v);   //插入栈内存写入
    void InsertExprForRead(Expression* e,int v);    //插入栈内存读取
    int GenTempVariable();              //获取一个新临时变量的id


    void InitFunEnv();                  //函数入口形参处理
    int insert();                       //添加一个int变量到栈中
    void frameFree();                   //释放栈的最后一个空间
    void genFlow();                     //生成程序流
    void OptimizeFlow();                //优化程序流(常数传播)
    void OptimizeDead();                //死代码消除
    void OptimizeLoadStore();           //优化
    void InitializeVectorSpace();       //初始化
    void SaveReg();                     //call语句出保存调用者保存的寄存器

    //GenCode
    string opstring(int op);
    string opinstruct(int op);
    void OutputArithRIMul(int reg1,int reg2,unsigned imm);
    void GenCode();
    void GenRiscv64();
    void GenRiscv32();
    void checkReturn();

    //Debug
    void DebugPrint();
    template<class T>
    void DebugPrint(T & v);
    void DebugPrintColorResult();
    void DebugPrintPhysicsResult();
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
    void ReportWarning(string s);
    void process();
    //void GenPutGet();
    void GenGlobalTigger();
    void GenGlobal();
    Analyz();
};
extern int target;//0 for tigger; 1 for RiscV64
extern Analyz AnalyzInstance;
#endif
