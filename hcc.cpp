#include "hcc.h"
#include "symbol.h"
enum NodeType //语法树节点所对应语句类型
{
DualArith=0,
DualLogic=1,
Assign=2,
SingleArith=3,
SingleLogic=4,
If=5,
IfElse=6,
While=7,
Bracket=8,
Return=9,
Vardfn=10,
Fundec=11,
Fundfn=12,
MainFun=13,
ArrayAssign=14,
//Identifier=15,
Symbol = 15,
Params=16,
//Immediate=17,
Funcall=18,
ArrayItem=19,
};
struct Node //语法树节点
{
    Node* left;
    Node* right;
    NodeType type;
    int val; //指示运算符或直接数数值
    Symbol* symbol;
    int line; //对应源代码中的行数
    Node(Node* _left,Node* _right,NodeType _type,int _val,int _line=0):left(_left),right(_right),type(_type),val(_val),line(_line){}
};
