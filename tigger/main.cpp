#include "tigger.h"
#include "y.tab.h"
#include "analyz.h"
Analyz Analyz::Instance = Analyz();
int main()
{
    yyparse();
    Analyz::Instance.process();
    return 0;
}