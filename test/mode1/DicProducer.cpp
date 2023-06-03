#include "DicProducer.hpp"
#include "SplitTool.hpp"
#include "Configuration.hpp"
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <locale>
#include <codecvt>

#define CONFIGPATH "myconf.conf"
using std::endl;
using std::ofstream;
using std::ifstream;

DicProducer::DicProducer(SplitTool *splitTool)
:_splitTool(splitTool)
{
    {
        string dir = "./data/art_CN";
        DIR *pdir = opendir(dir.c_str());
        if (pdir == nullptr)
        {

            return;
        }

        struct dirent *pdirent;
        while ((pdirent = readdir(pdir)) != nullptr)
        {
            if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
                continue;
            _files.push_back(dir + "/" + pdirent->d_name);
        }
        closedir(pdir);
    }

    { 
    string dir2 = "./data/art_EN";
    DIR *pdir = opendir(dir2.c_str());
    if (pdir == nullptr)
    {

        return;
    }
    struct dirent *pdirent;
    while ((pdirent = readdir(pdir)) != nullptr)
    {
        if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
            continue;
        _files2.push_back(dir2 + "/" + pdirent->d_name);
    }
    closedir(pdir);
    }

    buildCnDict();
    storeCnDict();


    buildEnDict();
    storeEnDict();
    _files.clear();
    _files2.clear();
    _dict.clear();
    _index.clear();

}
bool isChinese(wchar_t c)
{
    return (c >= 0x4E00 && c <= 0x9FFF) || (c >= 0x3400 && c <= 0x4DBF);
}

bool isFirstChinese(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(str);

    if (wstr.empty())
    {
        return false;
    }

    wchar_t c = wstr[0];
    if (!iswalpha(c))
    {
        if (isChinese(c))
        {
            return true;
        }
    }

    return false;
}

void DicProducer::buildCnDict()
{
    // 读取配置文件

    _stopWordPath = Configuration::getInstance(CONFIGPATH)->getConfigMap()["stopWordCN"];
    map<string, int> dict;
    // 读取停用词
    ifstream fs(_stopWordPath);
    if (!fs.good())
    {
        return;
    }
    // 停用词集合
    set<string> stopWord;
    string word;
    while (fs >> word)
    {
        stopWord.insert(word);
    }
    fs.close();
    // dbg(stopWord);
    // 去除停用词
    for (auto &filename : _files)
    {
        ifstream fs(filename);
        if (!fs.good())
        {
            return;
        }

        string line;
        while (getline(fs, line))
        {
            vector<string> words = _splitTool->cut(line);
            for (auto &word : words)
            {
                if (!isFirstChinese(word))
                {
                    continue;
                }
                if (stopWord.find(word) != stopWord.end())
                {
                    continue;
                }
                if (dict.find(word) != dict.end())
                {
                    dict[word]++;
                }
                else
                {
                    dict[word] = 1;
                }
            }
        }
        fs.close();
    }
    // 将词频放入vector中
    _dict.clear();
    _index.clear();
    _dict = vector<pair<string, int>>(dict.begin(), dict.end());

    int lineNum = 0;
    for (auto &elemt : _dict)
    {
        string word = elemt.first;
        ++lineNum;

        auto it = word.begin();
        while (it != word.end())
        {
            if (isFirstChinese(word))
            {
                string oneChineseWord = word.substr(0, 3);
                _index[oneChineseWord].insert(lineNum);
                word.erase(0, 3);
                _index[oneChineseWord].insert(lineNum);
            }
            it + 3;
        }
    }
}


bool isEnglish(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(str);

    if (wstr.empty())
    {
        return false;
    }

    for(char c :str){
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
    {
    }else{
        return false;
    }

    }

    return true;
}



void DicProducer::buildEnDict()
{
    //读取配置文件
    string stopWordPath = Configuration::getInstance(CONFIGPATH)->getConfigMap()["stopWordEN"];
    map<string, int> dict;
    //读取停用词
    ifstream fs(stopWordPath);
    if (!fs.good())
    {
        return;
    }
    //停用词集合
    set<string> stopWord;
    string word;
    while (fs >> word)
    {
        stopWord.insert(word);
    }
    fs.close();
    // 去除停用词
    for (auto &filename : _files2)
    {
        ifstream fs(filename);
        if (!fs.good())
        {
            return;
        }

        string line;
        while (getline(fs, line))
        {   
            vector<string> words = _splitTool->cut(line);
            for (auto &word : words)
            {
                
                if(!isEnglish(word))
                {
                    continue;
                }
                for(auto &c : word)
                {
                    if(iswalpha(c))
                    {
                        c = tolower(c);
                    }
                }

                if (stopWord.find(word) != stopWord.end())
                {
                    continue;
                }
                if(word.size() < 2)
                {
                    continue;
                }
                if (dict.find(word) != dict.end())
                {
                    
                    dict[word]++;
                }
                else
                {
                    dict[word] = 1;
                }
            }
        
        }
        fs.close();
    }
    // 将词频放入vector中
    _dict.clear();
    _index.clear();
    _dict = vector<pair<string,int>>(dict.begin(),dict.end());

    int lineNum = 0;
    for (auto &elemt : _dict)
    {
        string word = elemt.first;
        ++lineNum;

        char *p = (char *)word.c_str();
        while (*p != '\0')
        {
            
            string oneword(1,*p);;
            _index[oneword].insert(lineNum);

            ++p;
        
        } 
    }

}

void DicProducer::storeEnDict()
{
    string enDictPath = Configuration::getInstance(CONFIGPATH)->getConfigMap()["enDictPath"];
    ofstream ofs(enDictPath);
    if(!ofs.good())
    {
        return;
    }

    for(auto &elemt : _dict)
    {
        ofs << elemt.first << " " << elemt.second << endl;
    }
    ofs.close();

    string enIndexPath = Configuration::getInstance(CONFIGPATH)->getConfigMap()["enIndexPath"];
    ofstream ofs2(enIndexPath);
    if(!ofs2.good())
    {
        return;
    }

    for(auto &elemt : _index)
    {
        ofs2 << elemt.first << " ";
        for(auto &lineNum : elemt.second)
        {
            ofs2 << " " << lineNum;
        }
        ofs2 << endl;
    }
    ofs2.close();
   
}

void DicProducer::storeCnDict()
{
    string cnDictPath = Configuration::getInstance(CONFIGPATH)->getConfigMap()["cnDictPath"];
    ofstream ofs(cnDictPath);
    if (!ofs.good())
    {
        return;
    }

    for(auto &elemt : _dict)
    {
        ofs << elemt.first << " " << elemt.second << endl;
    }
    ofs.close();

    string cnIndexPath = Configuration::getInstance(CONFIGPATH)->getConfigMap()["cnIndexPath"];
    ofstream ofs2(cnIndexPath);
    if (!ofs2.good())
    {
        return;
    }
    for (auto &elemt : _index)
    {
        ofs2 << elemt.first << " ";
        for (auto &lineNum : elemt.second)
        {
            ofs2 << " " << lineNum;
        }
        ofs2 << endl;
    }
    ofs2.close();
}


DicProducer::~DicProducer()
{
}
