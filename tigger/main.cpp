#include "tigger.h"
#include "y.tab.h"
#include "analyz.h"
#include <string.h>
Analyz AnalyzInstance;
int target;
int main(int argc , char** argv)
{
    yyparse();
    target = 1;
    if(argc>1 && strcmp(argv[1],"-tg") == 0) target = 0;
    else if(argc>1 && strcmp(argv[1],"-m32")==0)  target = 2;
    AnalyzInstance.process();
    return 0;
}