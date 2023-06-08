//
// Created by ryan on 23-6-6.
//

#include "../include/WebPageQuery.hpp"
#include "../include/json.hpp"
#include <fstream>
#include <sstream>
#include "../include/dbg.h"

using std::ifstream;
using std::istringstream;
using std::make_pair;

WebPageQuery::WebPageQuery()
{
    loadLibrary();

}


void WebPageQuery::loadLibrary()
{
    string pageLibPath = "../data/ripepage.dat";
    string offsetLibPath = "../data/offset.dat";
    string invertIndexTablePath = "../data/invertindex.dat";
    ifstream pageLibIn(pageLibPath);
    ifstream offsetLibIn(offsetLibPath);
    ifstream invertIndexTableIn(invertIndexTablePath);
    if(!pageLibIn.good() || !offsetLibIn.good() || !invertIndexTableIn.good())
    {
        cout << "open file error" << endl;
        dbg(pageLibIn.good());
        dbg(offsetLibIn.good());
        dbg(invertIndexTableIn.good());
        return;
    }
    string line;
    //读取偏移库
    while(getline(offsetLibIn, line))
    {
        istringstream iss(line);
        int docId, offset, length;
        iss >> docId >> offset >> length;
        _offsetLib.insert(make_pair(docId, make_pair(offset, length)));
    }
    //读取网页库
    for(auto & item : _offsetLib)
    {
        int docId = item.first;
        int offset = item.second.first;
        int length = item.second.second;
        pageLibIn.seekg(offset, pageLibIn.beg);
        char *buff = new char[length + 1]();
        pageLibIn.read(buff, length);
        string doc(buff);
        WebPage webPage(doc, &_splitTool);
        _pageLib.insert(make_pair(docId, webPage));
        delete [] buff;
    }
    //读取倒排索引表
    while(getline(invertIndexTableIn, line))
    {
        istringstream iss(line);
        string word;
        iss >> word;
        int docId;
        double weight;
        while(iss >> docId >> weight)
        {
            _invertIndexTable[word].insert(make_pair(docId, weight));
        }
    }



    pageLibIn.close();
    offsetLibIn.close();
    invertIndexTableIn.close();


}
//执行查询
string WebPageQuery::doQuery(string query)
{
    //分词
    vector<string> queryWords = _splitTool.cut(query);
    dbg(queryWords);
    //计算查询词的权重
    vector<double> queryWordsWeight = getQueryWordsWeight(queryWords);
    dbg(queryWordsWeight);
    //执行查询，获取候选文档
    vector<pair<int, vector<double>>> resultVec;
    dbg(resultVec);
    bool ret = executeQuery(queryWords, resultVec);
    if(!ret)
    {
        return returnNoAnswer();
    }


    //计算基准向的
    double base = 0.0;
    for(auto &item : queryWordsWeight)
    {
        base += item * item;
    }

    base = sqrt(base);

    //计算相似度
    for(auto &item : resultVec)
    {
        double sum = 0.0;
        for(size_t i = 0; i != queryWords.size(); ++i)
        {
            sum += item.second[i] * queryWordsWeight[i];
        }
        double similarity = sum / base;
        item.second.push_back(similarity);
    }
    //对候选文档进行排序
    sort(resultVec.begin(), resultVec.end(), [](const pair<int, vector<double>> & lhs, const pair<int, vector<double>> & rhs){
        return lhs.second.back() > rhs.second.back();
    });
    
    //根据候选文档获取文档的内容打包成json格式
    vector<int> docIdVec;
    int i = 0;
    for(auto &item : resultVec)
    {
        docIdVec.push_back(item.first);
        
    }
    string result = createJson(docIdVec, queryWords);
    return result;

}
//查询查询词的权重
vector<double> WebPageQuery::getQueryWordsWeight(vector <std::string> &queryWords)
{
    vector<double> queryWordsWeight;
    for(auto &word : queryWords)
    {
        int df = _invertIndexTable[word].size();
        int N = _pageLib.size();
        double idf = log(static_cast<double>(N) / (df + 1)) / log(2);
        queryWordsWeight.push_back(idf);
    
    }
    return queryWordsWeight;
}
//执行查询，获取候选文档
bool WebPageQuery::executeQuery(const vector <std::string> &queryWords, vector <pair<int, vector < double>>> &resultVec)
{
    for(auto & word : queryWords)
    {
        auto it = _invertIndexTable.find(word);
        if(it == _invertIndexTable.end())
        {
            continue;
        }
        for(auto & item : it->second)
        {
            int docId = item.first;
            double weight = item.second;
            auto it2 = resultVec.begin();
            for(; it2 != resultVec.end(); ++it2)
            {
                if(docId == it2->first)
                {
                    break;
                }
            }
            if(it2 == resultVec.end())
            {
                resultVec.push_back(make_pair(docId, vector<double>{weight}));
            }
            else
            {
                it2->second.push_back(weight);
            }
        }
    }
    if(resultVec.empty())
    {
        return false;
    }
    return true;


}

//根据候选文档获取文档的内容打包成json格式
string WebPageQuery::createJson(vector<int> &docIdVec, const vector <std::string> &queryWords)
{
    using namespace nlohmann;
    if(docIdVec.empty())
    {
        //返回一个空的json数组
        json resultJson = R"([])"_json;
        json docJson = R"({})"_json;
        docJson["title"] = "dont have result";
        docJson["url"] = "  ";
        docJson["snippet"] = "   ";
        resultJson.push_back(docJson.dump());
        return json{{"candiWord", resultJson}}.dump();
    }
    size_t id = 0;
    json resultJson = R"([])"_json;
    for(const auto &docid : docIdVec)
    {
        json docJson = R"({})"_json;
        string Summary = _pageLib[docid].getDocSummary();
        if(Summary.empty())
        {
            Summary = _pageLib[docid].getDocContent();
        }
        if(Summary.empty())
        {
            Summary = _pageLib[docid].getDocTitle();
        }
        if(Summary.empty())
        {
            Summary = "无";
        }


        string Title = _pageLib[docid].getDocTitle();
        string Url = _pageLib[docid].getDocUrl();
        docJson["title"] = Title;
        docJson["url"] = Url;
        docJson["snippet"] = Summary;

        resultJson.push_back(docJson.dump());
    }
    return json{{"candiWord", resultJson}}.dump();
}
//返回无结果的提示信息
string WebPageQuery::returnNoAnswer()
{
    vector<int> docIdVec;
    vector<string> queryWords;
    return createJson(docIdVec, queryWords);
}
