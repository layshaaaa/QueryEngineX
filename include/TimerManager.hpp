#ifndef __TIMERMANAGER__
#define __TIMERMANAGER__

#include"TimerTask.hpp"
#include<unistd.h>
#include<sys/timerfd.h>

#include<iostream>
#include<vector>

using std::cout;
using std::endl;
using std::vector;

class TimerManager
{
public:
    TimerManager(int first_timeout,int timeout_gap);
    ~TimerManager();
    void start();
    void set_time(int first_timeout,int timeout_gap);

    void stop();
    void attach(TimerTask* task);

    void detach(TimerTask* task);
    
private:
    bool _isRunning;
    int _timerfd;
    vector<TimerTask*> _wheelList;
};

#endif
