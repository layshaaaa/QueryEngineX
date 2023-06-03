#ifndef __SPILITTOOL_HPP__
#define __SPILITTOOL_HPP__

#include <vector>
#include <string>
#include "../cppjieba/Jieba.hpp"

using std::vector;
using std::string;

class SplitTool:public cppjieba::Jieba
{
public:
    SplitTool(); 
    vector<string> cut(const string &sentence);
    
};



#endif
