#ifndef __x__
#define __x__
#include<memory>
#include<vector>
#include<map>
#include<set>
#include<iostream>
#include<fstream>
#include<sstream>
using std::cout;
using std::endl;
using std::ofstream;
using std::fstream;
using std::ifstream;
using std::map;
using std::pair;
using std::vector;
using std::string;
using std::set;
using std::istringstream;
using std::make_pair;



class Dictionary
{
public:
    static Dictionary* createInstance(){//单例模式获取对象函数
        if(_pDictionary==nullptr){
            _pDictionary=new Dictionary();
            atexit(destory);
        }
        return _pDictionary;
    }
    static void destory(){//单例模式销毁函数
        if(_pDictionary){
            delete _pDictionary;
            _pDictionary=nullptr;
        }
    }
    void init(const string &dictpath_dic,const string &dictpath_ind);//通过词典文件路径初始化词典
    vector<pair<string,int>>& getDict();//获取词典
    map<string ,set<int>>& getIndexTable();//获取索引表
    set<pair<string,int>> doQuery(string word);//执行查询,返回的set中，每个pair前面是包含查询子的词，后面是频率
private:
    Dictionary();//单例模式构造函数私有化
    Dictionary(const Dictionary&)=delete;//单例模式禁止拷贝构造
    Dictionary& operator=(const Dictionary)=delete;//单例模式禁止复制
private:
    static Dictionary* _pDictionary;//静态单例对象指针
    vector<pair<string,int>> _dict;
    map<string,set<int>> _indexTable;
    int _save;
public:
};
#endif
