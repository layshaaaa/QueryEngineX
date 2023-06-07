#ifndef __THREADCACHE__
#define __THREADCACHE__
#include<pthread.h>
#include<functional>
#include<iostream>

using std::function;
using std::cout;
using std::endl;
using ThreadCallBack=function<void()>;

class Thread_For_Cache
{
public:
    Thread_For_Cache(ThreadCallBack cb)
    :_tid(0)
    ,_cb(cb)
    {
        
    }

    void start()
    {
        int ret=pthread_create(&_tid,nullptr,threadFunc,this);
        if(ret!=0)
        {
            cout<<">>Thread_For_Cache start failed"<<endl;
            exit(1);
        }
    }

    void join()
    {
        pthread_join(_tid,nullptr);
    }
    static void* threadFunc(void* args)
    {
        Thread_For_Cache *pthis=(Thread_For_Cache*)args;
        if(pthis!=nullptr)
        {
            pthis->_cb();
        }

        pthread_exit(nullptr);
    }

    ~Thread_For_Cache()
    {

    }


private:
    pthread_t _tid;
    ThreadCallBack _cb;
};
#endif
