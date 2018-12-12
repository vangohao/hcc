#include "tigger.h"
#include "y.tab.h"
#include "analyz.h"
Analyz AnalyzInstance;
int target;
int main(int argc , char** argv)
{
    yyparse();
    target = 0;
    if(argc>1 && argv[1][0] =='1') target = 1;
    AnalyzInstance.process();
    return 0;
}