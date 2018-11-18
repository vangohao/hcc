#ifndef __SYMBOL_H
#define __SYMBOL_H
#include "tigger.h"
class Func
{
public:
    string name;
    int paraCount;
    static std::map<string,Func*> Map;
    Func(string _name,int _paraCount);
};


#endif