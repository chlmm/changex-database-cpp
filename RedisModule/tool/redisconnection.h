#ifndef REDISCONNECTION_H
#define REDISCONNECTION_H

#include <QObject>
#include <QString>
#include <sw/redis++/redis++.h>
#include <memory>

class RedisConnection
{
public:
    explicit RedisConnection();
    ~RedisConnection();

    /**
    * @brief 连接管理
    *
    * 连接到Redis服务器,断开与Redis服务器的连接
    * 检查是否已连接到Redis服务器,获取Redis客户端指针
    */
    bool connectToServer(const QString &host = "127.0.0.1", int port = 6379);
    void disconnect();
    bool isConnected() const;
    sw::redis::Redis* redis() const;

private:
    std::unique_ptr<sw::redis::Redis> redis_;
    bool connected_;
};

#endif // REDISCONNECTION_H
