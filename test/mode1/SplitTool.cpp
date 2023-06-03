#include "SplitTool.hpp"
#include <vector>

SplitTool::SplitTool()
:cppjieba::Jieba("../cppjieba/dict/jieba.dict.utf8",
                 "../cppjieba/dict/hmm_model.utf8",
                 "../cppjieba/dict/user.dict.utf8",
                 "../cppjieba/dict/idf.utf8",
                 "../cppjieba/dict/stop_words.utf8")
{

}

vector<string> SplitTool::cut(const string &sentence)
{
    vector<string> words;
    cppjieba::Jieba::CutForSearch(sentence,words);
    return words;
}
