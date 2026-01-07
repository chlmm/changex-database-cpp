#include "redisgenericoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisGenericOperations::RedisGenericOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisGenericOperations::~RedisGenericOperations()
{
}

bool RedisGenericOperations::del(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->del(key.toStdString());
        qDebug() << "DEL" << key;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "DEL error:" << e.what();
        return false;
    }
}

bool RedisGenericOperations::exists(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        bool result = connection_->redis()->exists(key.toStdString());
        qDebug() << "EXISTS" << key << "=" << result;
        return result;
    } catch (const std::exception &e) {
        qCritical() << "EXISTS error:" << e.what();
        return false;
    }
}

QVector<QString> RedisGenericOperations::keys(const QString &pattern)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::string> keys;
        connection_->redis()->keys(pattern.toStdString(), std::back_inserter(keys));
        for (const auto &key : keys) {
            result.append(QString::fromStdString(key));
        }
        qDebug() << "KEYS" << pattern << "found" << result.size() << "keys";
    } catch (const std::exception &e) {
        qCritical() << "KEYS error:" << e.what();
    }

    return result;
}
