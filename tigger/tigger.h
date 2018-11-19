#ifndef __TIGGER_H
#define __TIGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <initializer_list>
#include <set>
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::cerr;
using std::endl;
int yylex();
int yyerror(string s);
int calcarith(int a,int op,int b);
int calclogic(int a,int op,int b);
#endif