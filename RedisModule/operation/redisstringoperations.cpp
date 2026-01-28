#include "redisstringoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisStringOperations::RedisStringOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisStringOperations::~RedisStringOperations()
{
}

bool RedisStringOperations::set(const QString &key, const QString &value)
{
    return execute([&]() {
        connection_->redis()->set(key.toStdString(), value.toStdString());
        qDebug() << "SET [设置]" << key << "=" << value;
        return true;
    }, "SET", false);
}

QString RedisStringOperations::get(const QString &key)
{
    return execute([&]() {
        auto value = connection_->redis()->get(key.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "GET [获取]" << key << "=" << result;
            return result;
        }
        qDebug() << "GET [获取]" << key << "= (null)";
        return QString();
    }, "GET", QString());
}
