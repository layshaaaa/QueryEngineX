#include "Configuration.hpp"
#include <fstream>





using std::fstream;

Configuration::Configuration(const string &filepath){
    fstream fs(filepath);
    if (!fs.good())
    {
        cout << "fs.good() == false" << endl;
        return;
    }

    string key,value;
    while (fs >> key >> value)
    {
        key = key.substr(0,key.size()-1);
        _confMap[key] = value;
    } 

    fs.close();
}


map<string,string> &Configuration::getConfigMap(){
    return _confMap;
}

Configuration::~Configuration(){
    if (_pInstance){
        delete _pInstance;
        _pInstance = nullptr;
    }
}

Configuration* Configuration::getInstance(const string &filepath)
{
    if (_pInstance == nullptr)
    {
        _pInstance = new Configuration(filepath);
    }
    return _pInstance;
}





Configuration::AutoRelease::AutoRelease()
{
}
Configuration::AutoRelease::~AutoRelease()
{
    if (_pInstance)
        delete _pInstance;
}





Configuration *Configuration::_pInstance = nullptr;
Configuration::AutoRelease Configuration::_auto;
