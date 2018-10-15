#include<sstream>
#include<vector>
#include "gotolist.h"
#include "node.h"
#include "symbol.h"
Label::Label()
    {
        id = -1;
    }
void Label::Init(int x)
    {
        location = x;
        id = -1;
    }
std::string Label::print()
    {
        if(id == -1)
        {
            id = usedCount++;
        }   
        std::stringstream ss;
        ss<<"l"<<id;
        return ss.str();
    }
void Label::Patch()
    {
        Output::patch(location,*this,1);
    }
Node::Node(Node* _left,Node* _right,NodeType _type,
    int _val,
    int _line):

        left(_left),right(_right),
        type(_type),val(_val),line(_line){}
