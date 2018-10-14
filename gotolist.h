#include<list>
//#include"node.h"
struct Label;
namespace Output
{
    void patch(int i,Label& l);
}
struct Gotolist
{
    std::list<int> list;
    Gotolist(){}
    Gotolist(int i)
    {
        list = std::list<int>();
        list.push_back(i);
    }
    Gotolist merge(Gotolist & l2)
    {
        Gotolist g1;
        g1.list = std::list<int>(list);
        g1.list.merge(l2.list);
        return g1;
    }
    void backpatch(Label& l)
    {
        for(auto x: list)
        {
            Output::patch(x,l);
        }
    }
};