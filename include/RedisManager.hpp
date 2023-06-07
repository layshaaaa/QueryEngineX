#ifndef REDISMANAGER_HPP
#define REDISMANAGER_HPP

#include <vector>
#include <string>
#include <hiredis/hiredis.h>

class RedisManager {
public:
    // 获取 RedisManager 的单例对象
    static RedisManager& getInstance();

    // 从 Redis 中获取指定 key 的数据
    // index 表示使用哪个 Redis 连接
    std::string getData(int index, const std::string& key);

    void setData(const std::string &key, const std::string &value);

    // 向 Redis 中设置指定 key 的数据
    // index 表示使用哪个 Redis 连接

private:
    // 构造函数和析构函数为私有，禁止外部创建对象和销毁对象
    RedisManager();
    ~RedisManager();

    // 禁止拷贝构造函数和赋值运算符
    RedisManager(const RedisManager&) = delete;
    RedisManager& operator=(const RedisManager&) = delete;

    std::string host_; // Redis 服务器的主机名或 IP 地址
    int port_; // Redis 服务器的端口号
    int poolSize_; // 连接池的大小
    std::vector<redisContext*> connections_; // Redis 连接池
};

#endif // REDISMANAGER_HPP


