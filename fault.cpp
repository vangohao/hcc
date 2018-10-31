#include<string>
#include<iostream>
#include <stdio.h>
#include "fault.h"
bool errorstatus = false;
int yyerror(std::string msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<" at line "<<yylineno<<"."<<std::endl;
errorstatus = true;
return 0;
}
int yyerror(const char * msg)            //输出错误信息的yyerror()函数
{
std::cerr<<"Error encountered: "<<msg<<" at line "<<yylineno<<"."<<std::endl;
errorstatus = true;
return 0;
}