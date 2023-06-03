#include<vector>
#include<string>
#include<memory>
#include"Jieba.hpp"

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
    CnSplitTool(const char*  DICT_PATH="jieba.dict.utf8",
                         const char*  HMM_PATH="hmm_model.utf8",
                         const char*  USER_DICT_PATH="user.dict.utf8",
                         const char*  IDF_PATH="idf.utf8",
                         const char*  STOP_WORD_PATH="stop_words.utf8");
    virtual vector<string> cut_words(const string &line);
private:
    unique_ptr<cppjieba::Jieba> _up;
};
