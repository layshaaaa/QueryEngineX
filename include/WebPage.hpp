#ifndef __WEBPAGE_HPP__
#define __WEBPAGE_HPP__

#include "SplitTool.hpp"
#include "Configuration.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>

using std::string;
using std::map;
using std::vector;
using std::set;
class WebPage
{
public:
    WebPage() = default; 
    WebPage(string doc,SplitTool *jieba);
    WebPage &operator=(const WebPage &rhs);
    int getDocId();
    string getDoc();
    string getDocTitle();
    string getDocUrl();
    string getDocContent();
    string getDocSummary();
    vector<string> &getTopWords();
    vector<string> &getWordsVec();
    map<string,double> &getWordsWeightMap();
    map<string,int> &getWordsMap();
    //计算哈希
    void calcHash();
    void processDoc();
    void calcTopK(vector<string> &wordsVec,int k,set<string> &stopWordList);
    friend bool operator==(const WebPage &lhs,const WebPage &rhs);
    friend bool operator<(const WebPage &lhs,const WebPage &rhs);
    const static int TOPK_NUMBER = 20;
private:
    bool isSpace(const string &word);
    bool isPunc(char ch);
private:
    uint64_t _u;
    string _doc;
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    string _docSummary;
    vector<string> _topWords;
    map<string,double> _wordsWeightMap;
    vector<std::pair<string,double>> _wordsWeightVec;
    map<string,int> _wordsMap;
    Configuration *_config;
    SplitTool *_jieba;
    vector<string> _wordsVec;//分词结果

};

#endif // __WEBPAGE_HPP__

