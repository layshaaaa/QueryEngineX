#ifndef __CACHEMANAGER__
#define __CACHEMANAGER__
#include "LRUCache.hpp"

#include<vector>

using std::vector;
class CacheManager
{
public:
    static CacheManager* getInstance(int cache_nums=4,int capcity=10);
    static void destroy();
    static void autoRelease();


    void initCache(const string &filename);//只让第一个cache通过initCache完成初始化
    void periodicUpdateCaches();
    LRUCache & getCache(int index);
private:
    CacheManager(int cache_nums,int capcity);
    ~CacheManager();
private:
    vector<LRUCache> _caches;
    
    static CacheManager* _pInstance;
};
#endif
