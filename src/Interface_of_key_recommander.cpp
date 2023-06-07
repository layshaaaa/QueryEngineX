#include"../include/Interface_of_key_recommander.hpp"


Interface_of_key_recommander::Interface_of_key_recommander(string& key)
    :_pBook(Dictionary::createInstance())
    ,_find(KeyRecommander(key))
{}

void Interface_of_key_recommander::init()
{
    /* _pBook->init("cnDict.txt","cnIndex.txt"); */
    /* _pBook->init("enDict.txt","enIndex.txt"); */
}
 
vector<string> Interface_of_key_recommander::get_recommand()
{
    return(_find.get_result());
}

