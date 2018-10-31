#ifndef __FAULT_H
#define __FAULT_H
extern bool errorstatus;
extern int yylineno;
int yyerror(std::string msg);
int yyerror(const char * msg);
#endif
