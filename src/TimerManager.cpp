#include"../include/TimerManager.hpp"
#include <fcntl.h>
#include <cstring>

TimerManager::TimerManager(int first_timeout,int timeout_gap)
:_isRunning(false)
{
    _timerfd=timerfd_create(CLOCK_REALTIME,0);
    if(_timerfd<0)
    {
        cout<<">> TimerManager::TimerManager create timerfd failed"<<endl;
        exit(1);
    }

    set_time(first_timeout,timeout_gap);
}

void TimerManager::set_time(int first_timeout,int timeout_gap)
{
    //设置时间
    struct itimerspec value;

    value.it_value.tv_sec=first_timeout;
    value.it_value.tv_nsec=0;

    value.it_interval.tv_sec=timeout_gap;
    value.it_interval.tv_nsec=0;

    int ret=timerfd_settime(_timerfd,0,&value,nullptr);
    if(ret<0)
    {
        cout<<">>TimerManager::set_time failed"<<endl;
        /* exit(1); */
    }
}

TimerManager::~TimerManager()
{
    close(_timerfd);
}

void TimerManager::start()
{
    _isRunning=true;
    while(_isRunning)
    {
        uint64_t one=0;
        int ret=read(_timerfd,&one,sizeof(one));
        if(ret!=sizeof(one))
        {
            cout<<">>TimerManager::start read failed"<<endl;
            std::cerr << "Failed to read timerfd: " << strerror(errno) << std::endl;
            exit(1);
        }
#if 1
        for(auto &task:_wheelList)
        {
            task->process();
        }
#endif
    }
}


void TimerManager::stop()
{
    _isRunning=false;
    set_time(0,0);
}

void TimerManager::attach(TimerTask* task)
{
    if(task==nullptr)
    {
        cout<<">>TimerManager::attach task is nullptr "<<endl;
        return;
    }
    _wheelList.push_back(task);
}

void TimerManager::detach(TimerTask* task)
{
    vector<TimerTask*>::iterator it=_wheelList.begin();
    while(it!=_wheelList.end())
    {
        if(*it==task)
        {
            _wheelList.erase(it);
            break;
        }
    }
    if(it==_wheelList.end())
    {
        cout<<">>TimerManager::detach task erase failed"
            <<"you need sure the task if exsit"<<endl;
        return;
    }
}


