#include "redisexpirationoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisExpirationOperations::RedisExpirationOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisExpirationOperations::~RedisExpirationOperations()
{
}

bool RedisExpirationOperations::expire(const QString &key, int seconds)
{
    return execute([&]() {
        bool result = connection_->redis()->expire(key.toStdString(), seconds);
        qDebug() << "EXPIRE" << key << seconds << "=" << result;
        return result;
    }, "EXPIRE", false);
}

bool RedisExpirationOperations::expireAt(const QString &key, qint64 timestamp)
{
    return execute([&]() {
        bool result = connection_->redis()->expireat(key.toStdString(), timestamp);
        qDebug() << "EXPIREAT" << key << timestamp << "=" << result;
        return result;
    }, "EXPIREAT", false);
}

int RedisExpirationOperations::ttl(const QString &key)
{
    return execute([&]() {
        long long ttlValue = connection_->redis()->ttl(key.toStdString());
        qDebug() << "TTL" << key << "=" << ttlValue;
        return static_cast<int>(ttlValue);
    }, "TTL", -1);
}

bool RedisExpirationOperations::persist(const QString &key)
{
    return execute([&]() {
        bool result = connection_->redis()->persist(key.toStdString());
        qDebug() << "PERSIST" << key << "=" << result;
        return result;
    }, "PERSIST", false);
}
