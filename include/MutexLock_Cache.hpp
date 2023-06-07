#ifndef __MUTEXLOCKCACHE__
#define __MUTEXLOCKCACHE__
#include<pthread.h>

class MutexLock_Cache
{
public:
    MutexLock_Cache()
    {
        pthread_mutex_init(&_mutex,nullptr);
    }

    ~MutexLock_Cache()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
private:
    pthread_mutex_t _mutex;
};
#endif
