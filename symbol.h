#ifndef __SYMBOL_H
#define __SYMBOL_H
#include<iostream>
#include<sstream>
#include<map>
#include<string>
#include<vector>
#include"node.h"
enum SymbolType //符号类型
{
    Int=0,
    IntPtr=1,
    FunPtr=2,
    Immediate=3,
    Logic=4,
    Error=5,
};
struct Symbol //符号
{
    static int origCount;
    static int tempCount;
    int id;
    int tempid;
    SymbolType type;
    int val; //附加值，函数参数个数或数组大小
    Node* appear; //第一次出现的位置
    Node* paramlist; //若为函数,指向它的参数表
    bool decleared;
    bool defined;
    int paramCount; //p0,p1这样的变量计数
    int immediate;
    std::string funName; //函数名称
    Symbol();
    Symbol(Node* _appear);
    Symbol(SymbolType _type); //创建临时符号
    Symbol(SymbolType _type, int _immediate); //创建直接数符号
    void print();
    static Symbol* ProcessDualOp(Node* n1,Node* n2,const char * x);
    static Symbol* ProcessSingleOp(Node* n1,const char * x);
    void Declear(SymbolType _type,int _paramCount=-1, int _val=0);
    void Define(SymbolType _type,int _paramCount, int _val);
    void DefineParamList(Node * pl);
    void Define();
    void ReportUndecleared();
    void CallWithParams(Node * n1);
private:
    bool checkParams(Node *n1);
};

struct SymbolTable
{
    std::map<std::string,Symbol*> table;
    SymbolTable * prev;
    SymbolTable(SymbolTable * st);
    void put(const char * s, Symbol* sym);
    Symbol* get(const char * s);
    Symbol* gethere(const char * s);
};
#endif