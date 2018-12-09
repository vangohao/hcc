#include "tigger.h"
#include "y.tab.h"
#include "analyz.h"
Analyz Analyz::Instance = Analyz();
unordered_map<int,int> frameArrayTable;
int target;
int main(int argc , char** argv)
{
    yyparse();
    target = 0;
    if(argc>1 && argv[1][0] =='1') target = 1;
    Analyz::Instance.process();
    return 0;
}