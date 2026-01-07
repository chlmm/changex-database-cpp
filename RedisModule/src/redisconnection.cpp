#include "redisconnection.h"
#include <QDebug>
#include <stdexcept>

RedisConnection::RedisConnection()
    : connected_(false)
{
}

RedisConnection::~RedisConnection()
{
    disconnect();
}

bool RedisConnection::connectToServer(const QString &host, int port)
{
    try {
        QString connectionOptions = "tcp://" + host + ":" + QString::number(port);
        redis_ = std::make_unique<sw::redis::Redis>(connectionOptions.toStdString());
        connected_ = true;
        qDebug() << "Connected to Redis server:" << host << ":" << port;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "Failed to connect to Redis:" << e.what();
        connected_ = false;
        return false;
    }
}

void RedisConnection::disconnect()
{
    if (redis_) {
        redis_.reset();
    }
    connected_ = false;
}

bool RedisConnection::isConnected() const
{
    return connected_;
}

sw::redis::Redis* RedisConnection::redis() const
{
    return redis_.get();
}
