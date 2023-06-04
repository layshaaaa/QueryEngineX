#include<vector>
#include<string>
#include<memory>
#include"cppjieba/Jieba.hpp"

using std::vector;
using std::string;
using std::unique_ptr;

class DictProducer;
class SplitTool
{
public:
    virtual vector<string> cut_words(const string& line)=0;
    virtual ~SplitTool()
    {

    }
};

class EnSplitTool
:public SplitTool
{
public:
    vector<string> cut_words(const string &line) override;
};

class CnSplitTool //使用cppJieba实现
:public SplitTool
{
public:
    CnSplitTool(const char*  DICT_PATH="../data/dict/jieba.dict.utf8",
                         const char*  HMM_PATH="../data/dict/hmm_model.utf8",
                         const char*  USER_DICT_PATH="../data/dict/user.dict.utf8",
                         const char*  IDF_PATH="../data/dict/idf.utf8",
                         const char*  STOP_WORD_PATH="../data/dict/stop_words.utf8");
    virtual vector<string> cut_words(const string &line);
private:
    unique_ptr<cppjieba::Jieba> _up;
};
//nihao
