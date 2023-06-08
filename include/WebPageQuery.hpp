//
// Created by ryan on 23-6-6.
//

#ifndef MODE4_WEBPAGEQUERY_HPP
#define MODE4_WEBPAGEQUERY_HPP
#include <vector>
#include <string>
#include <map>
#include "SplitTool.hpp"
#include "WebPage.hpp"

using std::unordered_map;
using std::vector;
using std::pair;
using std::string;
class WebPageQuery {
public:
    WebPageQuery();
    void loadLibrary();
    string doQuery(string query);
    vector<double> getQueryWordsWeight(vector<string> & queryWords);
    bool executeQuery(const vector<string> & queryWords, vector<pair<int, vector<double>>> & resultVec);
    string createJson(vector<int> & docIdVec, const vector<string> & queryWords);
    string returnNoAnswer();
private:
    SplitTool _splitTool;
    unordered_map<int, WebPage> _pageLib;//网页库
    unordered_map<int, pair<int, int>> _offsetLib;//偏移库
    unordered_map<string, set<pair<int, double>>> _invertIndexTable;//倒排索引表



};


#endif //MODE4_WEBPAGEQUERY_H
