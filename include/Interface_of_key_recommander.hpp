#ifndef __INTERFACE__
#define __INTERFACE__
#include"../include/KeyRecommander.hpp"




class Interface_of_key_recommander
{
public:
    Interface_of_key_recommander(string& key);
    vector<string> get_recommand();
    void init();
private:
    Dictionary * _pBook;
    KeyRecommander  _find;
};



#endif
