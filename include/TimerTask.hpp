#ifndef __TIMERTASK__
#define __TIMERTASK__
#include"../include/CacheManager.hpp"
#include<iostream>

using std::cout;
using std::endl;

class TimerTask
{
public:
    virtual void process()=0;
};

class MyTimerTask
:public TimerTask
{
public:
    void process() override
    {
        CacheManager* cache_manager=CacheManager::getInstance();

        cache_manager->periodicUpdateCaches();
    }
};
#endif
