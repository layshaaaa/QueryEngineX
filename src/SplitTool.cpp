#include "../include/SplitTool.hpp"

SplitTool::SplitTool()
:cppjieba::Jieba("../data/dict/jieba.dict.utf8",
                 "../data/dict/hmm_model.utf8",
                 "../data/dict/user.dict.utf8",
                 "../data/dict/idf.utf8",
                 "../data/dict/stop_words.utf8")
{

}

vector<string> SplitTool::cut(const string &sentence)
{
    vector<string> words;
    cppjieba::Jieba::CutForSearch(sentence,words);
    return words;
}
