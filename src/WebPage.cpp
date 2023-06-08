#include "../include/WebPage.hpp"
#include "../include/Configuration.hpp"
#define LOGGER_LEVEL LL_WARN 

#include "../include/Simhash/Simhasher.hpp"
#include <map>
#include <string>
#include <set>
#include <vector>   
#include <map>

using std::map;
using std::multimap;
using std::greater;
using namespace simhash;

string jiebadictutf8 = Configuration::getInstance()->getConfigMap()["jiebadictutf8"];
string hmmmodelutf8 = Configuration::getInstance()->getConfigMap()["hmmmodelutf8"];
string idfutf8 = Configuration::getInstance()->getConfigMap()["idfutf8"];
string stopwordsutf8 = Configuration::getInstance()->getConfigMap()["stopwordsutf8"];

Simhasher simhasher(jiebadictutf8, hmmmodelutf8, idfutf8, stopwordsutf8);

WebPage::WebPage(string doc, SplitTool* jieba)
    : _doc(doc)
    , _docId(-1)
    , _docTitle("")
    , _docUrl("")
    , _docContent("")
    , _docSummary("")
    , _config(Configuration::getInstance())
    , _jieba(jieba)
{
    processDoc();
    // calcHash();
}

WebPage& WebPage::operator=(const WebPage& rhs)
{
    if(this != &rhs)
    {
        _doc = rhs._doc;
        _docId = rhs._docId;
        _docTitle = rhs._docTitle;
        _docUrl = rhs._docUrl;
        _docContent = rhs._docContent;
        _docSummary = rhs._docSummary;
        _topWords = rhs._topWords;
        _wordsWeightMap = rhs._wordsWeightMap;
        _wordsWeightVec = rhs._wordsWeightVec;
        _wordsMap = rhs._wordsMap;
        _config = rhs._config;
        _jieba = rhs._jieba;
        _wordsVec = rhs._wordsVec;
    }
    return *this;
} 

int WebPage::getDocId()
{
    return _docId;
}


string WebPage::getDoc()
{
    return _doc;
}

string WebPage::getDocTitle()
{
    return _docTitle;
}

string WebPage::getDocUrl()
{
    return _docUrl;
}

string WebPage::getDocContent()
{
    return _docContent;
}

string WebPage::getDocSummary()
{
    return _docSummary;
}

vector<string> &WebPage::getTopWords()
{
    return _topWords;
}

vector<string> &WebPage::getWordsVec()
{
    return _wordsVec;
}

map<string,int> &WebPage::getWordsMap()
{
    return _wordsMap;
}

map<string,double> &WebPage::getWordsWeightMap()
{
    return _wordsWeightMap;
}

void WebPage::processDoc()
{
    string docIdHead = "<docid>";
    string docIdTail = "</docid>";
    string docUrlHead = "<url>";
    string docUrlTail = "</url>";
    string docTitleHead = "<title>";
    string docTitleTail = "</title>";
    string docContentHead = "<content>";
    string docContentTail = "</content>";

    // 提取文档的 docId
    int bpos = _doc.find(docIdHead);
    int epos = _doc.find(docIdTail);
    string docId = _doc.substr(bpos + docIdHead.size(), epos - bpos - docIdHead.size());
    _docId = atoi(docId.c_str());

    // 提取文档的 docUrl
    bpos = _doc.find(docUrlHead);
    epos = _doc.find(docUrlTail);
    _docUrl = _doc.substr(bpos + docUrlHead.size(), epos - bpos - docUrlHead.size());

    // 提取文档的 docTitle
    bpos = _doc.find(docTitleHead);
    epos = _doc.find(docTitleTail);
    _docTitle = _doc.substr(bpos + docTitleHead.size(), epos - bpos - docTitleHead.size());

    // 提取文档的 docContent
    bpos = _doc.find(docContentHead);
    epos = _doc.find(docContentTail);
    _docContent = _doc.substr(bpos + docContentHead.size(), epos - bpos - docContentHead.size());

    // 提取文档的 docSummary
    string summary = _docContent;
    size_t pos = summary.find('\n');
    if (pos != string::npos) {
        summary.erase(pos);
    }
    
    _docSummary = summary;

    
    // 读取停用词
    string stopWordPath = _config->getConfigMap()["stop_word"];
    ifstream ifs(stopWordPath);
    if(!ifs.good())
    {
        cout << "ofstream open error!" << endl;
        return;
    }
    set<string> stopWordList;
    string stopWord;
    while(getline(ifs,stopWord))
    {
        stopWordList.insert(stopWord);
    }
    // 使用jieba分词
    _wordsVec = _jieba->cut(_docContent);
    

    // 计算词频

    calcTopK(_wordsVec, TOPK_NUMBER, stopWordList);

    // 计算词的权重
    // simhasher.extract(_docContent,_wordsWeightVec,TOPK_NUMBER);
    // simhasher.make(_docContent, TOPK_NUMBER, _u);
    
}

void WebPage::calcHash()
{
    // simhasher.extract(_docContent,_wordsWeightVec,TOPK_NUMBER);
    
    simhasher.make(_docContent, TOPK_NUMBER, _u);
}

// 判断是否是标点符号
bool WebPage::isPunc(char ch)
{
    string punc = "。，、；：？！“”‘’《》（）【】{}";
    for (size_t i = 0; i < punc.size(); i++)
    {
        if (ch == punc[i])
            return true;
    }
    return false;
}
// 判断字符串是否是空格
bool WebPage::isSpace(const string &word)
{
    for (size_t i = 0; i < word.size(); i++)
    {
        if (word[i] != ' ')
            return false;
    }
    return true;
}

void WebPage::calcTopK(vector<string> &wordsVec, int k, set<string> &stopWordList)
{
    // 定义一个 map 用于存储单词出现的次数
    map<string, int> wordCount;

    // 遍历 wordsVec 向量中的所有单词
    for (auto word : wordsVec)
    {
        // 将单词转换为小写字母
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        if (isPunc(word[0]))
            continue;
        if (isSpace(word))
            continue;
        // 如果单词不在停用词列表中，则将其添加到 wordCount map 中
        if (stopWordList.find(word) == stopWordList.end())
        {
            wordCount[word]++;
        }
    }

    // 定义一个 multimap 用于按照单词出现次数排序
    multimap<int, string, greater<int>> sortedWords;

    // 将 wordCount map 中的所有单词添加到 sortedWords multimap 中
    for (auto wc : wordCount)
    {
        sortedWords.insert(make_pair(wc.second, wc.first));
    }

    _wordsMap = wordCount;
    // dbg(_wordsMap);
}



bool operator==(const WebPage &lhs, const WebPage &rhs)
{
    return Simhasher::isEqual(lhs._u, rhs._u, 3); // 使用海明距离阈值为5进行比较
}

bool operator<(const WebPage &lhs, const WebPage &rhs)
{
    return lhs._docId < rhs._docId;
}
