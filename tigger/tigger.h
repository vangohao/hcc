#ifndef __TIGGER_H
#define __TIGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <initializer_list>
#include <set>
#include <algorithm>
#include <stack>
#include <list>
#include <sstream>
#include <queue>
#include <string.h>

using std::string;
using std::stringstream;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::set;
using std::cerr;
using std::endl;
using std::map;
using std::stack;
using std::list;
using std::cout;
using std::queue;

int yylex();
int yyerror(string s);
int calcarith(int a,int op,int b);
int calclogic(int a,int op,int b);
#endif