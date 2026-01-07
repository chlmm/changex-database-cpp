#include "redisexpirationoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisExpirationOperations::RedisExpirationOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisExpirationOperations::~RedisExpirationOperations()
{
}

bool RedisExpirationOperations::expire(const QString &key, int seconds)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        bool result = connection_->redis()->expire(key.toStdString(), seconds);
        qDebug() << "EXPIRE" << key << seconds << "=" << result;
        return result;
    } catch (const std::exception &e) {
        qCritical() << "EXPIRE error:" << e.what();
        return false;
    }
}

bool RedisExpirationOperations::expireAt(const QString &key, qint64 timestamp)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        bool result = connection_->redis()->expireat(key.toStdString(), timestamp);
        qDebug() << "EXPIREAT" << key << timestamp << "=" << result;
        return result;
    } catch (const std::exception &e) {
        qCritical() << "EXPIREAT error:" << e.what();
        return false;
    }
}

int RedisExpirationOperations::ttl(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return -1;
    }

    try {
        long long ttlValue = connection_->redis()->ttl(key.toStdString());
        qDebug() << "TTL" << key << "=" << ttlValue;
        return static_cast<int>(ttlValue);
    } catch (const std::exception &e) {
        qCritical() << "TTL error:" << e.what();
        return -1;
    }
}

bool RedisExpirationOperations::persist(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        bool result = connection_->redis()->persist(key.toStdString());
        qDebug() << "PERSIST" << key << "=" << result;
        return result;
    } catch (const std::exception &e) {
        qCritical() << "PERSIST error:" << e.what();
        return false;
    }
}
