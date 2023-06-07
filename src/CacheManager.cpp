#include"../include/CacheManager.hpp"

#include<pthread.h>

#include<fstream>
#include<iostream>

using std::cout;
using std::endl;
using std::ifstream;

CacheManager* CacheManager::getInstance(int cache_nums,int capcity)
{
    if(_pInstance==nullptr)
    {
        _pInstance=new CacheManager(cache_nums,capcity);
        pthread_once_t _once=PTHREAD_ONCE_INIT;
        pthread_once(&_once,autoRelease);
    }
    return _pInstance;
}
CacheManager::CacheManager(int cache_nums,int capcity)
{
    for(int i=0;i<cache_nums;++i)
    {
        _caches.push_back(LRUCache(capcity));
    }
}

void CacheManager::destroy()
{
    if(_pInstance!=nullptr)
    {
        delete _pInstance;
        _pInstance=nullptr;
    }
}
void CacheManager::autoRelease()
{
    atexit(CacheManager::destroy);
}


CacheManager::~CacheManager()
{
    
}

void CacheManager::initCache(const string &filename)
{
    if(_caches.size()==0)
    {
        cout<<"need run CacheManager::getInstance for Singleton"<<endl;
        return;
    }

    int ret=_caches[0].getFromFile(filename);
    if(ret!=0)
    {
        return;
    }
    
    for(int i=1;i<_caches.size();++i)
    {
        cout<<i<<endl;
        _caches[i].sync_with_cache1(_caches[0]);
    }
#if 0
    for(int i=0;i<_caches.size();++i)
    {
        _caches[i].show();
    }
#endif
}

void CacheManager::periodicUpdateCaches()
{
    for(int i=1;i<_caches.capacity();++i)
    {
        _caches[i].write_to_cache1(_caches[0]);
    }

    for(int i=1;i<_caches.capacity();++i)
    {
        _caches[i].sync_with_cache1(_caches[0]);
    }

    _caches[0].write_to_File();
}


LRUCache& CacheManager::getCache(int index)
{
    return _caches[index];
}

//线程数，最大容量
CacheManager* CacheManager::_pInstance=CacheManager::getInstance(4,10);

#if 0
int main (int argc, char *argv[]) {
    
    CacheManager* cache_manager=CacheManager::getInstance(4,10);
    cache_manager->initCache("../conf/cache_init.conf");
    
    cout<<"-------------------"<<endl;
    LRUCache &cache1=cache_manager->getCache(0);
    cache1.addRecord("d","c");
    cache1.show();

    LRUCache &cache2=cache_manager->getCache(1);
    cache2.addRecord("qi","qi");
    cache2.addRecord("bi","bi");

    cache_manager->periodicUpdateCaches();
    cache2.show();

    cache1.write_to_File();
#if 0
    LRUCache &cache3=cache_manager->getCache(2);
    LRUCache &cache4=cache_manager->getCache(3);
#endif
    //cache_manager->periodicUpdateCaches();
    return 0;
}
#endif
