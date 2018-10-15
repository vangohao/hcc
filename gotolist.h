#include<list>
//#include"node.h"
struct Label;
struct Gotolist
{
    std::list<int> list;
    Gotolist();
    Gotolist(int i);
    Gotolist merge(Gotolist & l2);
    void backpatch(Label& l);
};