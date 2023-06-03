#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <string>
#include <map>
#include <iostream>

using std::map;
using std::string;
using std::cout;
using std::endl;

class Configuration
{
public:
    Configuration(const string &filepath);
    ~Configuration();

    static Configuration* getInstance(const string &filepath);
    map<string,string> &getConfigMap();

    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };   
    
    
private:
    Configuration& operator =(const Configuration &rhs);
    Configuration(const Configuration &rhs);
    string _confFilePath;
    map<string,string> _confMap;
    static Configuration *_pInstance;
    static AutoRelease _auto;

};

#endif
