#ifndef __NODE_H
#define __NODE_H
#include<sstream>
#include<vector>
#include"gotolist.h"
struct Label;
namespace Output
{
    void patch(int,Label&,int);
}
enum NodeType //语法树节点所对应语句类型
{
ExprArith=0,
ExprLogic=1,
Assign=2,
SingleArith=3,
SingleLogic=4,
If=5,
IfElse=6,
While=7,
Bracket=8,
Return=9,
Vardfn=10,
Fundcl=11,
Fundfn=12,
MainFun=13,
ArrayAssign=14,
Symbol1 = 15,
Params=16,
Funcall=18,
ArrayItem=19,
VarDcl = 20,
Empty = 21,
Stmts = 22,
};
struct Symbol;
struct Label
{
    static int usedCount;       //使用的标号的计数(为了省略为使用的标号)
    int id;                     //标号id
    int location;               //位置
    Label();
    void Init(int x);           //初始化标号
    void Patch();               //完成对跳转到该标号的goto指令的回填
    std::string print();
};
struct Node //语法树节点
{
    Node* left;
    Node* right;
    NodeType type;
    int val; //指示运算符或直接数数值
    int line; //对应源代码中的行数
    Label instr;    //语句标号
    Gotolist nextlist;  //跳转表, 跳转到当前语句块之后的 goto指令集合
    Gotolist falselist; //跳转表, 跳转到当前语句块为真的位置的 goto指令集合
    Gotolist truelist;  //跳转表, 跳转到当前语句块为假的位置的 goto指令集合
    Symbol* sym;        //符号, 用于函数,变量,表达式,直接数
    Node(Node* _left,Node* _right,NodeType _type,
    int _val,
    int _line=0);
};

#endif