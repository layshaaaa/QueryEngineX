#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Socket.h"
#include "SocketIO.h"
#include "InetAddress.h"

#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;

class EventLoop;//前向声明

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
public:
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
    explicit TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection();
    void send(const string &msg);
    void sendInLoop(const string &msg);
    string receive();
    bool isClosed() const;
    string toString();

    //注册Tcp连接中的三个事情
    void setConnectionCallback(const TcpConnectionCallback &cb);
    void setMessageCallback(const TcpConnectionCallback &cb);
    void setCloseCallback(const TcpConnectionCallback &cb);

    //执行Tcp通信过程中的三个事件
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    //获取本端地址与对端地址
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

private:
    EventLoop *_loop;//让TcpConnection知道EventLoop的存在
    SocketIO _sockIO;

    //为了调试而加入的函数
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;

    TcpConnectionCallback _onConnectionCb;//连接建立
    TcpConnectionCallback _onMessageCb;//消息到达
    TcpConnectionCallback _onCloseCb;//连接断开
};

#endif
