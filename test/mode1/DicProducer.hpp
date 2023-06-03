#ifndef __DICPRODUCER_HPP__
#define __DICPRODUCER_HPP__

#include <string>
#include <vector>
#include <map>
#include <set>


class SplitTool;

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::set;

class DicProducer
{
public:
    DicProducer(SplitTool *splitTool);
    ~DicProducer();
    void buildEnDict();
    void buildCnDict();
    void storeEnDict() ;
    void storeCnDict() ;

private:
    vector<string> _files;
    vector<string> _files2;
    vector<pair<string,int>> _dict;
    map<string,set<int>> _index;
    string _stopWordPath;
    SplitTool *_splitTool;
};



#endif // __DICPRODUCER_H__
