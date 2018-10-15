#ifndef __NODE_H
#define __NODE_H
#include<sstream>
#include<vector>
#include "gotolist.h"
// struct Gotolist;
namespace Output
{
    void patch(int i,Label& l,int x);
}
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
Fundcl=11,
Fundfn=12,
MainFun=13,
ArrayAssign=14,
//Identifier=15,
Symbol1 = 15,
Params=16,
//Immediate=17,
Funcall=18,
ArrayItem=19,
VarDcl = 20,
Empty = 21,
Stmts = 22,
// Stmt = 23,
};
struct Symbol;
struct Label
{
    static int usedCount;
    int id;
    int location;
    Label()
    {
        id = -1;
    }
    void Init(int x)
    {
        location = x;
        id = -1;
    }
    std::string print()
    {
        if(id == -1)
        {
            id = usedCount++;
        }   
        std::stringstream ss;
        ss<<"l"<<id;
        return ss.str();
    }
    void Patch()
    {
        Output::patch(location,*this,1);
    }
};
struct Node //语法树节点
{
    Node* left;
    Node* right;
    NodeType type;
    int val; //指示运算符或直接数数值
    int line; //对应源代码中的行数
    Label instr;
    Gotolist nextlist;
    Gotolist falselist;
    Gotolist truelist;
    Symbol* sym;
    Node(Node* _left,Node* _right,NodeType _type,
    int _val,
    int _line=0):

        left(_left),right(_right),
        type(_type),val(_val),line(_line){}
};

#endif