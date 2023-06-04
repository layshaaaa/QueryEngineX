#include"DictProducer.hpp"
#include"splitTool.hpp"


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
    cout<<"sizeof _stopfile is : "<<_stopfile.size()<<endl;
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

void DictProducer::buildEnIndex(const string &dicfilepath)
{
    ifstream ifs(dicfilepath);
    if(!ifs.good())
    {
        cout<<"DictProducer::buildEnDict open failed"<<endl;
        exit(1);
    }
    
    string line;
    int nums=0;//行号
    while(std::getline(ifs,line))
    {
        istringstream iss(line);
        string word;
        iss>>word;

        for(auto &ch:word)
        {
            if(isalpha(ch))
            {
                string alphas(1,ch);
                _alpha_index[alphas].insert(nums+1);
            }
        }
        nums++;
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

void DictProducer::write_frequency_file(const string &filepath)
{
    ofstream ofs(filepath,std::ios_base::out);
    if(!ofs.good())
    {
        cout<<"DictProducer::write_frequency_file open failed"<<endl;
        exit(1);
    }
    for(auto &elem:_word_frequency) //elem是pair类型
    {
        ostringstream oss;
        
        if(_stopfile.find(elem.first)==_stopfile.end())
        {
            //elem是pair类型<string,int>
            oss<<elem.first<<" "<<elem.second;    
            ofs<<oss.str()<<endl;
        }
    }

}

void DictProducer::write_index_file(const string &filepath)
{
    ofstream ofs(filepath);
    if(!ofs.good())
    {
        cout<<"DictProducer::write_line_file open failed"<<endl;
        exit(1);
    }
    for(auto &pairs:_alpha_index)
    {
        ostringstream oss;
        
        oss<<string(pairs.first);

        for(auto &line_no:pairs.second)
        {
            oss<<" "<<line_no;
        }
        ofs<<oss.str()<<endl;
    }
}

