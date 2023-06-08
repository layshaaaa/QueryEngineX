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
    Configuration();
    ~Configuration();

    static Configuration* getInstance();
    map<string,string> &getConfigMap();

    class AutoRelease
    {
    public:
        AutoRelease();
        ~AutoRelease();
    };   
    
    static void destroy();
 
private:
    Configuration& operator =(const Configuration &rhs);
    Configuration(const Configuration &rhs);
    string _confFilePath;
    map<string,string> _confMap;
    static Configuration *_pInstance;
    static AutoRelease _auto;

};




#endif
