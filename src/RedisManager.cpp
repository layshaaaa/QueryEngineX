#include "RedisManager.hpp"
#include <iostream>
#include <string>
#include <string.h>

RedisManager::RedisManager()
    : host_("127.0.0.1"), port_(6379), poolSize_(4)
{
    connections_.reserve(poolSize_);

    // 初始化 Redis 连接池
    for (int i = 0; i < poolSize_; ++i) {
        redisContext* context = redisConnect(host_.c_str(), port_);
        if (context != nullptr && context->err == 0) {
            connections_.push_back(context);
        } else {
            // 连接 Redis 失败
            std::cerr << "Failed to connect to Redis server." << std::endl;
            if (context != nullptr) {
                std::cerr << "Error: " << context->errstr << std::endl;
                redisFree(context);
            }
        }
    }
}

RedisManager::~RedisManager()
{
    // 释放 Redis 连接池中的所有连接
    for (redisContext* context : connections_) {
        redisFree(context);
    }
}

RedisManager& RedisManager::getInstance()
{
    // 获取 RedisManager 的单例对象
    static RedisManager instance;
    return instance;
}

std::string RedisManager::getData(int index, const std::string& key)
{
    if (index < 0 || index >= poolSize_) {
        return "Invalid index.";
    }

    redisContext* context = connections_[index];

    // 从 Redis 中获取指定 key 的数据
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "GET %s", key.c_str()));
    if (reply != nullptr && reply->type == REDIS_REPLY_STRING) {
        std::string data(reply->str, reply->len);
        freeReplyObject(reply);
        return data;
    } else {
        if (reply != nullptr) {
            // 获取数据失败
            std::cerr << "Failed to get data from Redis: " << reply->str << std::endl;
            freeReplyObject(reply);
        } else {
            std::cerr << "Failed to get data from Redis." << std::endl;
        }
        return "Data not found.";
    }
}

void RedisManager::setData(const std::string& key, const std::string& value)
{
    redisContext* context = connections_[0];

    // 向 Redis 中设置指定 key 的数据
    redisReply* reply = static_cast<redisReply*>(redisCommand(context, "SET %s %s", key.c_str(), value.c_str()));
    if (reply != nullptr && reply->type == REDIS_REPLY_STATUS && strcmp(reply->str, "OK") == 0) {
        freeReplyObject(reply);
    } else {
        if (reply != nullptr) {
            // 设置数据失败
            std::cerr << "Failed to set data in Redis: " << reply->str << std::endl;
            freeReplyObject(reply);
        } else {
            std::cerr << "Failed to set data in Redis." << std::endl;
        }
    }
}