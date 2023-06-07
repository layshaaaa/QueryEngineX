#include"../include/Dictionary.hpp"
#include<string>
using std::string;


Dictionary* Dictionary::_pDictionary=Dictionary::createInstance();

Dictionary::Dictionary()
    :_save(0)
    ,_pMysql(EasyMysql::getInstance("localhost","root","NAN819"))
{
        //init("cnDict.txt","cnIndex.txt");
        //init("enDict.txt","enIndex.txt");
    }

/*void Dictionary::init(const string &dictpath_dic,const string &dictpath_ind)
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
}*/
set<pair<string,int>> Dictionary::doQuery(string word)
{   
    //------------------------------------------------------------------------
    /*set<int> tmp_set;//获得单个字母在字典中出现的所有行号
    for(auto &ele:_indexTable){
     feed_back   if(ele.first==word){
            tmp_set=ele.second;
        }
    }*/    
    //------------------------------------------------------------------------
    set<int> line_number;//放置行号的set

    int flag=0;
    char ch=word[0]; 
    string oooooooooooooooo;
    string xxxxxxxxxxxxxxxx;
    if(ch & (1<<7)){//看看传入的word是英文字母还是中文字
        int n=1;
        for(int i=0;i!=6;++i){
            if(ch & (1<<(6-i))){
                ++n;
            }
        }
        //不是英文
        flag=1;
        char str[100]={0};
        sprintf(str,"select word_line from cn_index where hanzi='%s'",word.c_str());
        oooooooooooooooo=str;
        vector<vector<string>> value=_pMysql->read_from_mysql(oooooooooooooooo);
        if(value.size()==0|value[0].size()>1000){return set<pair<string,int>>();}
        xxxxxxxxxxxxxxxx=(value[0])[0];//这个xxxxx是从数据库读出来的string，一会儿换
        /*istringstream iss_tmp(xxxxxxxx);
        while(!iss_tmp.eof()){
            string tmp_key;
            iss_tmp>>tmp_key;
            cout<<tmp_key<<endl;
            if(tmp_key.size()==0){break;}
            int num=std::stoi(tmp_key);
            line_number.insert(num);
        }*/

    }else{
        //英文
        flag=-1;
        char str[100]={0};
        sprintf(str,"select word_line from en_index where alpha='%s'",word.c_str());
        oooooooooooooooo=str;
        vector<vector<string>> value=_pMysql->read_from_mysql(oooooooooooooooo);
        if(value.size()==0|value[0].size()>1000){return set<pair<string,int>>();}
        xxxxxxxxxxxxxxxx=(value[0])[0];//这个xxxxx是从数据库读出来的string，一会儿换
        /*istringstream iss_tmp(xxxxxxxx);
        while(!iss_tmp.eof()){
            string tmp_key;
            iss_tmp>>tmp_key;
            cout<<tmp_key<<endl;
            if(tmp_key.size()==0){break;}
            int num=std::stoi(tmp_key);
            line_number.insert(num);
        }*/
    }


    if(!xxxxxxxxxxxxxxxx.empty()){
        xxxxxxxxxxxxxxxx.pop_back();
    }
    //-----------------------------------------------------------------------
    set<pair<string,int>> feed_back;
    if(flag==1){//中文
        /*for(auto &elem:line_number){
            char str[100]={0};
            sprintf(str,"select word,frequency from cn_dict where id=%d",elem);
            
            string oooooooooooooooo(str);
            //把ooo传进去查
            vector<vector<string>> end=_pMysql->read_from_mysql(oooooooooooooooo);
            if(end[0][0].size()>100|end[0][1].size()>100){
                break;
            }
        }*/
        string query_query="select word,frequency from cn_dict where id in ("+xxxxxxxxxxxxxxxx+")";
        vector<vector<string>> end=_pMysql->read_from_mysql(query_query);
        for(auto &elem:end){
            if(elem.size()==0|elem[0].size()>1000){break;}
            string x=elem[0];
            int y=std::stoi(elem[1]);
            feed_back.insert(make_pair(x,y));
        }
    }else if(flag==-1){//英文
        /*for(auto &elem:line_number){
            char str[100]={0};
            sprintf(str,"select word,frequency from en_dict where id=%d",elem);
            string oooooooooooooooo(str);
      //``      vector<vector<string>> end= {{"hello","12"}};
            cout<<"0"<<endl;
    
            vector<vector<string>> end = _pMysql->read_from_mysql(oooooooooooooooo);
            cout<<end[0][0].size()<<endl;
            cout<<end[0][1].size()<<endl;
            if(end[0][0].size()>100|end[0][1].size()>100){
                cout<<"ooooooooooooooo"<<endl;
                cout<<end[0][0]<<endl;
                cout<<"ooooooooooooooo"<<endl;
                cout<<end[0][1]<<endl;
                cout<<"ooooooooooooooo"<<endl;
                break;}
            
            cout<<"1"<<endl;
            string x=end[0][0];
            int y=std::stoi(end[0][1]);

            //把ooo传进去查

            //返回的结果，处理成一个string和一个int
            feed_back.insert(make_pair(x,y));
        }*/
        string query_query="select word,frequency from en_dict where id in ("+xxxxxxxxxxxxxxxx+")";
        vector<vector<string>> end=_pMysql->read_from_mysql(query_query);
        for(auto &elem:end){
            if(elem.size()==0|elem[0].size()>1000){break;}
            string x=elem[0];
            int y=std::stoi(elem[1]);
            feed_back.insert(make_pair(x,y));
        }
    }else{cout<<"未判断出中英文"<<endl;}



    //----------------------------------------------------------------------
    return feed_back;//返回的是所有单词和频率
}
