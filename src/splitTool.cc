#include"splitTool.hpp"
#include"Jieba.hpp"
#include<sstream>
#include<iostream>

using std::string;
using std::cout;
using std::endl;
using std::istringstream;


vector<string> EnSplitTool::cut_words(const string &line)
{
    vector<string> tmp;
    istringstream iss(line);
    string word;

    while(iss>>word)
    {
        tmp.push_back(word);
    }
    return tmp;
}


CnSplitTool::CnSplitTool(const char*  DICT_PATH,
                         const char*  HMM_PATH,
                         const char*  USER_DICT_PATH,
                         const char*  IDF_PATH,
                         const char*  STOP_WORD_PATH)
:_up(new cppjieba::Jieba(DICT_PATH,
                         HMM_PATH,
                         USER_DICT_PATH,
                         IDF_PATH,
                         STOP_WORD_PATH))
{

}
vector<string> CnSplitTool::cut_words(const string &line)
{
#if 0
    const char* const DICT_PATH = "jieba.dict.utf8";
    const char* const HMM_PATH = "hmm_model.utf8";
    const char* const USER_DICT_PATH = "user.dict.utf8";
    const char* const IDF_PATH = "idf.utf8";
    const char* const STOP_WORD_PATH = "stop_words.utf8";
#endif 
    vector<string> words;
    string line_cp(line); 
    _up->Cut(line_cp,words,true);

    return words;
}
