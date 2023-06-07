#include "../include/SearchEngineServer.h"
#include "../include/EasyMysql.hpp"
#include "../include/TimerManager.hpp"
#include "../include/Thread_For_Cache.hpp"
#include "../include/TimerTask.hpp"

#include <iostream>
using std::cout;
using std::endl;



int main(int argc, char **argv)
{
    
    MyTimerTask task;
    //定时一秒
    TimerManager Tmanager(1,1);
    Tmanager.attach(&task);
    Thread_For_Cache Tcache(std::bind(&TimerManager::start,std::ref(Tmanager)));
    Tcache.start();
    


    SearchEngineServer server(4, 10, "0.0.0.0", 8888);
    server.start();
    Tcache.join();

    return 0;
}

