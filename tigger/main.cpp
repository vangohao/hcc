#include "tigger.h"
#include "y.tab.h"
#include "analyz.h"
#include <string.h>
Analyz AnalyzInstance;
int target;
int main(int argc , char** argv)
{
    yyparse();
    target = 0;
    if(argc>1 && strcmp(argv[1],"-m64") == 0) target = 1;
    else if(argc>1 && strcmp(argv[1],"-m32")==0)  target = 2;
    AnalyzInstance.process();
    return 0;
}