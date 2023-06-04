#ifndef __y__
#define __y__
#include"Dictionary.hpp"
using std::shared_ptr;
using std::multiset;


class TcpConnection;
using TcpConnectionPtr=shared_ptr<TcpConnection>;
class KeyRecommander
{
friend struct ComparePoint;
public:
    void change_word(string &query);
    KeyRecommander(string &query);
    vector<string> get_result();//执行查询
    //1. 求取一个字符占据的字节数
    static size_t nBytesCode(const char ch);
    //2. 求取一个字符串的字符长度(其实是字符个数)
    static std::size_t length(const std::string &str);
    //3. 中英文通用的最小编辑距离算法
    static int editDistance(const std::string & lhs, const std::string &rhs);
    static int triple_min(const int &a,const int &b,const int &c);
private:
    Dictionary* _pDic;
    static string _queryWord;//等待查询的单词
};
int distance(const string &rhs,const string &lhs);

struct ComparePoint
    {
        bool operator()(const pair<string,int> &lhs,const pair<string,int> &rhs)const{
            if(distance(lhs.first,KeyRecommander::_queryWord)<distance(rhs.first,KeyRecommander::_queryWord)){
                return true;
            }else if(distance(lhs.first,KeyRecommander::_queryWord)==distance(rhs.first,KeyRecommander::_queryWord)){
                if(lhs.second>rhs.second){
                    return true;
                }else if(lhs.second==rhs.second){
                    if(lhs.first<rhs.first){
                        return true;
                    }else{return false;}
                }else{return false;}
            }else{return false;}
        }
    };
#endif
