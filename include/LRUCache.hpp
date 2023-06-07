#ifndef __LRUCache__
#define __LRUCache__
#include"MutexLock_Cache.hpp"

#include<list>
#include<map>
#include<string>

using std::string;
using std::list;
using std::map;

using Json=string;
using key=string;

struct CacheNode
{
    CacheNode(const key &ckey,const Json &cJson)
    :_ckey(ckey)
    ,_cJson(cJson)
    {

    }
    key _ckey;
    Json _cJson;
};

class LRUCache
{
public:
    LRUCache(int capcity=10);
    ~LRUCache();

    int addRecord(const key&,const Json&);
    Json readRecord(const key&);

    void write_to_cache1(LRUCache &cache1);
    void sync_with_cache1(const LRUCache &cache);
    const list<CacheNode>& getResutlt() const;
    const map<key,list<CacheNode>::iterator>& getKey_index() const;
    const list<CacheNode>& getNew_result_persec() const;

    int getFromFile(const string &filename="../conf/cache_init.conf");
    void write_to_File(const string &filename="../conf/cache_init.conf");
    
    void show();//测试方法
private:
    int _capcity; //cache容量
    list<CacheNode>  _result_list;
    map<key,list<CacheNode>::iterator> _key_index;
    list<CacheNode> _new_result_persec;
    
    MutexLock_Cache _mutex;
    int _ready_to_write;
};
#endif
