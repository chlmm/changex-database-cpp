#include "redisgenericoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisGenericOperations::RedisGenericOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisGenericOperations::~RedisGenericOperations()
{
}

bool RedisGenericOperations::del(const QString &key)
{
    return execute([&]() {
        connection_->redis()->del(key.toStdString());
        qDebug() << "DEL" << key;
        return true;
    }, "DEL", false);
}

bool RedisGenericOperations::exists(const QString &key)
{
    return execute([&]() {
        bool result = connection_->redis()->exists(key.toStdString());
        qDebug() << "EXISTS" << key << "=" << result;
        return result;
    }, "EXISTS", false);
}

QVector<QString> RedisGenericOperations::keys(const QString &pattern)
{
    return execute([&]() {
        std::vector<std::string> keys;
        connection_->redis()->keys(pattern.toStdString(), std::back_inserter(keys));
        QVector<QString> result;
        for (const auto &key : keys) {
            result.append(QString::fromStdString(key));
        }
        qDebug() << "KEYS" << pattern << "找到" << result.size() << "个键";
        return result;
    }, "KEYS", QVector<QString>());
}
