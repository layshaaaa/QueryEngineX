#include"../include/KeyRecommander.hpp"


void KeyRecommander::change_word(string &query){
    _queryWord=query;
}

 string KeyRecommander::_queryWord("我");
KeyRecommander::KeyRecommander(string &query)
    :_pDic(Dictionary::createInstance())
{
    _queryWord=query;
}
vector<string> KeyRecommander::get_result(){
    set<pair<string,int>,ComparePoint> good;


    int len=length(_queryWord);
    for(int i=0,index=0;i<len;++i,++index){
        int nBytes=KeyRecommander::nBytesCode(_queryWord[index]);
        string target=_queryWord.substr(index,nBytes);
        index+=(nBytes-1);
        set<pair<string,int>> yes=_pDic->doQuery(string(target));

        

        
        auto it=yes.begin();
            while(it!=yes.end()){
                if(distance((*it).first,_queryWord)>3){
                    it=yes.erase(it);
                }else{
                    ++it;
                }
            }
        


        good.insert(yes.begin(),yes.end());
    }
    vector<string> result;
    int count=0;
    for(auto &ele:good){
        result.push_back(ele.first);
        count++;
        if(count==10){
            break;
        }
    }
    if(count<10){
        for(int x=0;x<10-count;x++){
            result.push_back("无无无无无无无无无无");
        }
    }
    return result;
}
size_t KeyRecommander::nBytesCode(const char ch)
{
    if(ch & (1 << 7))
    {
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx)
        {
            if(ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }
            else
                break;
        }
        return nBytes;
    }
    return 1;
}
std::size_t KeyRecommander::length(const std::string &str)
{
    std::size_t ilen = 0;
    for(std::size_t idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}
int KeyRecommander::triple_min(const int &a, const int &b, const int &c)
{//返回三者间最小值
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}
int distance(const string &rhs,const string &lhs){
    size_t lhs_len = KeyRecommander::length(lhs);
    size_t rhs_len = KeyRecommander::length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for(size_t idx = 0; idx <= lhs_len; ++idx)
    {
        editDist[idx][0] = idx;
    }
    for(size_t idx = 0; idx <= rhs_len; ++idx)
    {
        editDist[0][idx] = idx;
    }
    std::string sublhs, subrhs;
    for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i,
        ++lhs_idx)
    {
        size_t nBytes = KeyRecommander::nBytesCode(lhs[lhs_idx]);//lhs的第lhs_idx+1个字符的字节数
        sublhs = lhs.substr(lhs_idx, nBytes);//把lhs的第1(2(3(4(...))))个字符提取出来
        lhs_idx += (nBytes - 1);//lhs_idx往后偏移(指向)第二个字符开头位置
        for(std::size_t dist_j = 1, rhs_idx = 0;
            dist_j <= rhs_len; ++dist_j, ++rhs_idx)
        {
            nBytes = KeyRecommander::nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            //以上三句提取了rhs的第1(2(3(4(...))))个字符
            if(sublhs == subrhs)
            {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j -
                    1];
            }
            else
            {
                editDist[dist_i][dist_j] =
                    KeyRecommander::triple_min(editDist[dist_i][dist_j - 1] + 1,
                               editDist[dist_i - 1][dist_j] + 1,
                               editDist[dist_i - 1][dist_j - 1] + 1);
            }
        }
    }
    return editDist[lhs_len][rhs_len];    
}

