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
ExprArith=0,            //数值表达式
ExprLogic=1,            //逻辑表达式
Assign=2,               //赋值语句
If=5,                   //If语句
IfElse=6,               //IfElse语句
While=7,                //While语句
Return=9,               //Return语句
Vardfn=10,              //变量定义语句
Fundcl=11,              //函数声明语句
Fundfn=12,              //函数定义语句
MainFun=13,             //main函数
ArrayAssign=14,         //数组赋值语句
Symbol1 = 15,           //未定类型的临时符号
Params=16,              //参数表(调用时)
Funcall=18,             //函数调用语句
VarDcl = 20,            //参数表(定义或声明时)
Empty = 21,             //空类型
Stmts = 22,             //程序体
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