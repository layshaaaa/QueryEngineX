#include"../include/DictProducer_with_mysql.hpp"
#include"../include/splitTool.h"
#include"../include/EasyMysql.hpp"

#include<unistd.h>
#include<dirent.h>
#include<string.h>


#include <locale>
#include <codecvt>
#include<fstream>
#include<iostream>
#include<sstream>

using std::ostringstream;
using std::istringstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

DictProducer::DictProducer(const string &dirpath)
:_filepath()
,_stopfile()
,_word_frequency()
,_alpha_index()
{
    //to read 目录流
    readDirpath(dirpath);
}

DictProducer::DictProducer(const string &dirpath,SplitTool *splittool)
:_filepath()
,_stopfile()
,_word_frequency()
,_alpha_index()
,_cuttor(splittool)
{
    readDirpath(dirpath);
}

DictProducer::~DictProducer()
{
    
}

void DictProducer::readDirpath(const string &dirpath)
{
    _filepath.reserve(64);//先给_filepath预留64个元素的空间
    

    DIR* pdir=::opendir(dirpath.c_str());
    if(pdir==nullptr)
    {
        cout<<"opendir failed"<<endl;
        exit(1);
    }


    errno=0;
    while(1)
    {
        struct dirent* pdirent=readdir(pdir);
        if(pdirent==nullptr)
        {
            if(errno!=0)
            {
                perror("readdir");
                exit(1);
            }
            break;
        }


         // cout<<pdirent->d_name<<endl;
        if(strcmp(pdirent->d_name,".")!=0 && strcmp(pdirent->d_name,"..")!=0)
        {
            //路径的拼接
            char tmp[256]={0};
            ::strcpy(tmp,dirpath.c_str());
            ::strcat(tmp,pdirent->d_name); 
            _filepath.push_back(tmp);
        }
        
    }
    closedir(pdir);
}

void DictProducer::show()
{
    for(auto &path:_filepath)
    {
        cout<<path<<endl;
    }
    cout<<"vector<filepath> size is : "<<_filepath.size()<<endl;
}

vector<string> DictProducer::readFile(const string &filepath)
{
    using line=string;
    vector<line> tmp; 
    ifstream ifs(filepath);//按照绝对路径打开文件流
    if(!ifs.good())
    {
        cout<<"DictProducer::readFile open failed"<<endl;
        exit(1);
    }

    string lines;
    while(std::getline(ifs,lines))
    {
        Preprocess(lines);
        tmp.push_back(lines);
    }

    return tmp;//将文件按行读入内存保存到vector
}

vector<string> DictProducer::readCnFile(const string &filepath)
{
    using line=string;
    vector<line> tmp; 
    ifstream ifs(filepath);//按照绝对路径打开文件流
    if(!ifs.good())
    {
        cout<<"DictProducer::readFile open failed"<<endl;
        exit(1);
    }

    string lines;
    while(std::getline(ifs,lines))
    {
        tmp.push_back(lines);
    }

    return tmp;//将文件按行读入内存保存到vector
}

void DictProducer::readStopFile(const string &filepath)
{
    ifstream ifs(filepath);
    if(!ifs.good())
    {
        cout<<"DictProducer::readStopFile open failed"<<endl;
        exit(1);
    }

    string line;
    while(std::getline(ifs,line))
    {
        istringstream iss(line);
        
        string word;
        while(iss>>word)
        {
            _stopfile.insert(word);
        }
    }
}


void DictProducer::Preprocess(string &line)
{
    for(auto &alpha:line)
    {
        if(!std::isalpha(alpha))
        {
            alpha=' ';
        }
        else {
            alpha=tolower(alpha);
        }
    }
}

void DictProducer::buildEnDict()
{
    _cuttor=new EnSplitTool();
    for(auto &filepath:_filepath)
    {
        using line=string;
        vector<line> file_content=readFile(filepath);

        for(auto &lines:file_content)
        {
            using word=string;
            vector<word> tmp;
            if(_cuttor!=nullptr)
            {
                tmp=_cuttor->cut_words(lines);
            }
            for(auto &words:tmp)
            {
                ++_word_frequency[words];//统计词频
            }
        }

    }
    
    // if(_cuttor!=nullptr)
    // {
    //     delete _cuttor;
    // }
}

#if 1
void DictProducer::buildEnIndex()
{
    ifstream ifs("../data/dict/en_dict.dat");
    if(!ifs.good())
    {
        cout<<">>DictProducer::buildEnDict open failed"<<endl;
        exit(1);
    }

    string line;
    int line_no=0;
    while(std::getline(ifs,line))
    {
        istringstream iss(line);
        string word;
        iss>>word;

        for(auto &ch:word)
        {
            string ch_str(1,ch);
            _alpha_index[ch_str].insert(line_no+1);
        }
        line_no++;
    }
}
#endif
void DictProducer::write_frequency_mysql(bool switchs)//true=写英文词典库，false=写中文词典库
{
    EasyMysql* easymysql=EasyMysql::getInstance("localhost","root","123");

    cout<<"build word-frequency dictionary"<<endl;
    if(switchs)
    {
         for(auto &pairs:_word_frequency)
         {
             if(_stopfile.find(pairs.first)==_stopfile.end())
             {
                  char command[128]={0}; 
                  char buf[16]={0};
                  ::strcpy(buf,std::to_string(pairs.second).c_str());
                  ::sprintf(command,"insert into en_dict (word,frequency) values ('%s',%s);",pairs.first.c_str(),buf);
             
                 easymysql->write_to_mysql(command);
             }
         }

        cout<<"build en-word-frequency-dictionart finish"<<endl;
    }
    else{
         for(auto &pairs:_word_frequency)
         {
             if(_stopfile.find(pairs.first)==_stopfile.end())
             {
                  char command[128]={0}; 
                  char buf[16]={0};
                  ::strcpy(buf,std::to_string(pairs.second).c_str());
                  ::sprintf(command,"insert into cn_dict (word,frequency) values ('%s',%s);",pairs.first.c_str(),buf);
             
                 easymysql->write_to_mysql(command);
             }
         }

        cout<<"build cn-word-frequency-dictionart finish"<<endl;
    }
}

void DictProducer::write_index_mysql(bool switchs)
{
    EasyMysql* easymysql=EasyMysql::getInstance("localhost","root","123");

    if(switchs)
    {

         for(auto &pairs:_alpha_index)
         {
             ostringstream oss;
             for(auto &line:pairs.second) //每个字母对应的行号
             {
                oss<<line<<","; 
             }
             
             string str(oss.str());

             int len=pairs.first.size()+str.size();
             char *command=new char[len+128]();

             sprintf(command,"insert into en_index (alpha,word_line) values ('%s','%s');",pairs.first.c_str(),str.c_str());
             easymysql->write_to_mysql(command);
             delete []command;
         }
    }
    else{
         for(auto &pairs:_alpha_index)
         {
             ostringstream oss;
             for(auto &line:pairs.second) //每个字母对应的行号
             {
                oss<<line<<","; 
             }
             
             string str(oss.str());

             int len=pairs.first.size()+str.size();
             char *command=new char[len+128]();

             sprintf(command,"insert into cn_index (hanzi,word_line) values ('%s','%s');",pairs.first.c_str(),str.c_str());
             easymysql->write_to_mysql(command);
             delete []command;
         }
    }
    
}

std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;

bool isChineseCharacter(char32_t c) {
    // 判断字符是否为汉字
    // 汉字的Unicode编码范围为 0x4E00 - 0x9FFF
    return (c >= 0x4E00 && c <= 0x9FFF);
}

bool isSpecialChineseCharacter(char32_t c) {
    // 定义特殊中文字符的条件
    // 这里只是一个示例，你可以根据自己的需求进行修改
    // 假设特殊中文字符的Unicode编码范围为 0xFF00 - 0xFFEF
    return (c >= 0xFF00 && c <= 0xFFEF);
}

std::string filterChineseCharacters(const std::string& input) {
    std::u32string u32_input = converter.from_bytes(input);
    std::u32string filtered;
    for (char32_t c : u32_input) {
        if (isChineseCharacter(c) && !isSpecialChineseCharacter(c)) {
            filtered += c;
        }
    }
    return converter.to_bytes(filtered);
}
void DictProducer::buildCnDict()
{
    for(auto &filepath:_filepath)
    {
        using line=string;
        vector<line> file_content=readCnFile(filepath);//把一个文件完整读入内存

        for(auto &lines:file_content)
        {
            using word=string;
            vector<word> tmp=_cuttor->cut_words(lines);

            for(auto &words:tmp)
            {
                string temp=filterChineseCharacters(words);
                if(temp.size()!=0)
                {
                    ++_word_frequency[words];
                }            
            }
        }
    }
}

void DictProducer::buildCnIndex(const string &dicfilepath)//词库文件的位置
{
    ifstream ifs(dicfilepath);
    if(!ifs.good())
    {
        cout<<"DictProducer::buildCnIndex open failed"<<endl;
        exit(1);
    }
    

    string line;
    int nums=0;//行号
    while(std::getline(ifs,line))
    {
        istringstream iss(line);
        string cizu;
        iss>>cizu;

       int index=0;
       char buf[3];
       while(index<cizu.size())
       {
            buf[0]=cizu[index++];
            buf[1]=cizu[index++];
            buf[2]=cizu[index++];

            if((buf[0] & 0xE0)==0xE0)
            {
                _alpha_index[buf].insert(nums+1);
            } 
       }

       nums++;
    }
}



