#include"../include/Thread_For_Cache.hpp"
#include"../include/TimerManager.hpp"

int main (int argc, char *argv[]) {
    
    MyTimerTask task;

    TimerManager manager(1,2);
    manager.attach(&task);

    Thread_For_Cache threadc(std::bind(&TimerManager::start,&manager));

    // sleep(10);

    threadc.start(); 
    threadc.join();
    return 0;
}
