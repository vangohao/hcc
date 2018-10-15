#include<list>
#include"gotolist.h"
#include"node.h"
namespace Output
{
    void patch(int i,Label& l,int x =0);
}
Gotolist::Gotolist(){}
Gotolist::Gotolist(int i)
    {
        list = std::list<int>();
        list.push_back(i);
    }
Gotolist Gotolist::merge(Gotolist & l2)
    {
        Gotolist g1;
        g1.list = std::list<int>(list);
        g1.list.merge(l2.list);
        return g1;
    }
void Gotolist::backpatch(Label& l)
    {
        if(!list.empty()) l.Patch();
        for(auto x: list)
        {
            Output::patch(x,l);
        }
    }