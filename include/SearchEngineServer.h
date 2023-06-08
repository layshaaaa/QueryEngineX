#ifndef __SearchEngineServer_H__
#define __SearchEngineServer_H__

#include "TcpServer.h"
#include "TcpConnection.h"
#include "ThreadPool.h"
#include "Interface_of_key_recommander.hpp"
#include "CacheManager.hpp"
#include "WebPageQuery.hpp"
#include "RedisManager.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include <unistd.h>


using std::cout;
using std::endl;
using json = nlohmann::json;

WebPageQuery w;

class MyTask
{
public:
    MyTask(const string &msg,int flag,const TcpConnectionPtr &con)
    : _msg(msg)
    , _con(con)
    ,_flag(flag)
    {
    }

    void process()
    {
        int threadID = std::stoi(name);
        if(1 == _flag)
        {   
            //获得cacheManager对象
            CacheManager * manager = CacheManager::getInstance();
            auto &cache = manager->getCache(threadID);
            
            string readyMessage;


            //先判断缓存是否有结果
            
            string result = cache.readRecord(_msg);
            if(result.size() == 0)
            {   
                cout << "没走缓存" <<endl;

                //查询关键词操作
                Interface_of_key_recommander keyRes(_msg);
                vector<string> candiWord;
                candiWord = keyRes.get_recommand();
                //打包候选词集发送给客户端
                json message;
                message["candiWord"] = candiWord;
                //已经完全打包好要发送的信息
                readyMessage = packIntoTrain(message.dump());//加了一个消息长度的小火车
                cache.addRecord(_msg,readyMessage);

            }else
            {
                readyMessage = result;
                cout << "走了缓存" << endl;
            }
            cout << readyMessage <<endl;



            _con->sendInLoop(readyMessage);


        }
        else
        {
            RedisManager *pRedis = &RedisManager::getInstance();
            //定义蓄势待发的信息
            string readyMessage;
            readyMessage = pRedis->getData(threadID,_msg);
            //先查询Redis缓存有没有
            if(readyMessage.size() != 0)
            {
                //Redis中有数据
                _con->sendInLoop(packIntoTrain(readyMessage));
                cout << "走了缓存" << endl;

            }
            else
            {
                //Redis中没有数据
                string retMessage = w.doQuery(_msg);
                _con->sendInLoop(packIntoTrain(retMessage));
                //将数据存放到Redis中
                pRedis->setData(_msg,retMessage);
                cout << "没走缓存" << endl;
            }

        }
    }
    std::string packIntoTrain(const std::string packedData) {
        std::string packedString;

        // 获取字符串的长度
        std::size_t size = packedData.size();

        // 将长度存储在前四个字节中
        char sizeBuf[4];
        sizeBuf[0] = static_cast<char>((size >> 24) & 0xFF);
        sizeBuf[1] = static_cast<char>((size >> 16) & 0xFF);
        sizeBuf[2] = static_cast<char>((size >> 8) & 0xFF);
        sizeBuf[3] = static_cast<char>(size & 0xFF);

        // 添加长度字节到packedString中
        packedString.append(sizeBuf, 4);

        // 添加packedData到packedString中
        packedString.append(packedData);

        return packedString;
    }



private:
    string _msg;
    int _flag;
    TcpConnectionPtr _con;
};

class SearchEngineServer
{
public:
    SearchEngineServer(size_t threadNum, size_t queSize
                       , const string &ip,
                       unsigned short port)
        : _pool(threadNum, queSize)
          , _server(ip, port)
    {
    }

    ~SearchEngineServer()
    {

    }


    void start()
    {
        _pool.start();//子线程会在TaskQueue中取Task，因为现在TaskQueue是空的，所以会调用pop阻塞

        //将Tcp通信过程中的三个事件注册给了TcpServer，然后交给EventLoop，最后交给TcpConnection
        using namespace std::placeholders;
        _server.setAllCallback(std::bind(&SearchEngineServer::onConnection, this, _1)
                               , std::bind(&SearchEngineServer::onMessage, this, _1)
                               , std::bind(&SearchEngineServer::onClose, this, _1));
        _server.start();
    }

    void stop()
    {
        _pool.stop();
        _server.stop();
    }

    void unpackMessage(const string &packedMessage,int &ID,string &content)
    {
        json message = json::parse(packedMessage);    
        ID = message["msgID"].get<int>();
        content = message["msgContent"].get<string>();
    }

    //连接建立
    void onConnection(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has connected!!!" << endl;
    }

    void onMessage(const TcpConnectionPtr &con)
    {
        string packedMessage = con->receive();
        cout << packedMessage.size() << endl;
        if(packedMessage.size() < 3)
        {
            return ;
        }
        cout << "packedMessage = " << packedMessage << endl;
        //解包  
        int msgID = 0;
        string msgContent;
        unpackMessage(packedMessage,msgID,msgContent); 
        if(1 == msgID)
        {
            //关键字推荐
            cout << "msgID = " << msgID << " msgContent = " << msgContent << endl;
            MyTask task(msgContent,msgID,con);
            _pool.addTask(std::bind(&MyTask::process, task));
        }
        else if(2 == msgID)
        {
            //网页搜索
            MyTask task(msgContent,msgID,con);
            _pool.addTask(std::bind(&MyTask::process, task));

        }
        else
        {
            perror("msgID");
            return;
        }
    }

    void onClose(const TcpConnectionPtr &con)
    {
        cout << con->toString() << " has closed!!!" << endl;
    }

private:
    ThreadPool _pool;
    TcpServer _server;

};


#endif
