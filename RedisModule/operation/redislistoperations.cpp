#include "redislistoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisListOperations::RedisListOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisListOperations::~RedisListOperations()
{
}

bool RedisListOperations::lPush(const QString &key, const QString &value)
{
    return execute([&]() {
        connection_->redis()->lpush(key.toStdString(), value.toStdString());
        qDebug() << "LPUSH" << key << value;
        return true;
    }, "LPUSH", false);
}

QString RedisListOperations::lPop(const QString &key)
{
    return execute([&]() {
        auto value = connection_->redis()->lpop(key.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "LPOP" << key << "=" << result;
            return result;
        }
        qDebug() << "LPOP" << key << "= (null)";
        return QString();
    }, "LPOP", QString());
}

bool RedisListOperations::rPush(const QString &key, const QString &value)
{
    return execute([&]() {
        connection_->redis()->rpush(key.toStdString(), value.toStdString());
        qDebug() << "RPUSH" << key << value;
        return true;
    }, "RPUSH", false);
}

QString RedisListOperations::rPop(const QString &key)
{
    return execute([&]() {
        auto value = connection_->redis()->rpop(key.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "RPOP" << key << "=" << result;
            return result;
        }
        qDebug() << "RPOP" << key << "= (null)";
        return QString();
    }, "RPOP", QString());
}

QVector<QString> RedisListOperations::lRange(const QString &key, int start, int stop)
{
    return execute([&]() {
        std::vector<std::string> values;
        connection_->redis()->lrange(key.toStdString(), start, stop, std::back_inserter(values));
        QVector<QString> result;
        for (const auto &value : values) {
            result.append(QString::fromStdString(value));
        }
        qDebug() << "LRANGE" << key << start << stop << "找到" << result.size() << "个元素";
        return result;
    }, "LRANGE", QVector<QString>());
}

int RedisListOperations::lLen(const QString &key)
{
    return execute([&]() {
        long long len = connection_->redis()->llen(key.toStdString());
        qDebug() << "LLEN" << key << "=" << len;
        return static_cast<int>(len);
    }, "LLEN", 0);
}

QString RedisListOperations::lIndex(const QString &key, int index)
{
    return execute([&]() {
        auto value = connection_->redis()->lindex(key.toStdString(), index);
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "LINDEX" << key << index << "=" << result;
            return result;
        }
        qDebug() << "LINDEX" << key << index << "= (null)";
        return QString();
    }, "LINDEX", QString());
}
