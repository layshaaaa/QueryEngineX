#include"Dictionary.hpp"


Dictionary* Dictionary::_pDictionary=Dictionary::createInstance();

Dictionary::Dictionary()
    :_save(0){
        init("cn_dict.dat","cn_index.dat");
        init("en_dict.dat","en_index.dat");
    }

void Dictionary::init(const string &dictpath_dic,const string &dictpath_ind)
{
    int last_save=_save;//保存上次的行数
    ifstream iss(dictpath_ind);//先读索引表
    if(!iss.good()){
        cout<<"open failed index!"<<endl;
        return;
    }
    string long_line;
    while(getline(iss,long_line)){//一行一行读
        string key;
        istringstream iss_tmp(long_line);
        iss_tmp>>key;//先把第一个单词拿出来
        set<int> tmp_set;
        while(!iss_tmp.eof()){//再循环读后面的行号
            string line_num;
            iss_tmp>>line_num;
            int num=std::stoi(line_num);
            num+=last_save;//注意把所有行号加上次的行数
            tmp_set.insert(num);//插入set中
        }
        _indexTable.insert(make_pair(key,tmp_set));//把key和set放入map后，继续读下一行
    }
    iss.close();



    ifstream ifs(dictpath_dic);//开始读字典
    if(!ifs.good()){
        cout<<"open failed dictionary!"<<endl;
        return;
    }
    string line;
    while(getline(ifs,line)){//一行一行读
         string key,value;
         istringstream ifs_tmp(line);
         ifs_tmp>>key>>value;//单词和次数分别放key和value中
         int num=std::stoi(value);
         _dict.push_back(make_pair(key,num));//插入字典中
         ++_save;//记得计算行数
    }
    ifs.close();//关文件
}
vector<pair<string,int>>& Dictionary::getDict()
{
    return _dict;
}
map<string ,set<int>>& Dictionary::getIndexTable()
{
    return _indexTable;
}
set<pair<string,int>> Dictionary::doQuery(string word)
{   
    set<int> tmp_set;
    for(auto &ele:_indexTable){
        if(ele.first==word){
            tmp_set=ele.second;
        }
    }    
    




    set<pair<string,int>> feed_back;
    for(auto &ele:tmp_set){
        feed_back.insert(make_pair(_dict[ele-1].first,_dict[ele-1].second));
    }
    return feed_back;
}
