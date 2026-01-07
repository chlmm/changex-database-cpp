#ifndef REDISCONNECTION_H
#define REDISCONNECTION_H

#include <QObject>
#include <QString>
#include <sw/redis++/redis++.h>
#include <memory>

class RedisConnection
{
public:
    /**
     * @brief 构造函数,初始化RedisConnection对象
     */
    explicit RedisConnection();

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisConnection();

    /**
     * @brief 连接到Redis服务器
     * @param host 服务器主机地址,默认为"127.0.0.1"
     * @param port 服务器端口号,默认为6379
     * @return 连接是否成功
     */
    bool connectToServer(const QString &host = "127.0.0.1", int port = 6379);

    /**
     * @brief 断开与Redis服务器的连接
     */
    void disconnect();

    /**
     * @brief 检查是否已连接到Redis服务器
     * @note 此函数不修改对象状态
     * @return 是否已连接
     */
    bool isConnected() const;

    /**
     * @brief 获取Redis客户端指针
     * @note 此函数不修改对象状态
     * @return Redis客户端指针
     */
    sw::redis::Redis* redis() const;

private:
    std::unique_ptr<sw::redis::Redis> redis_;
    bool connected_;
};

#endif // REDISCONNECTION_H
