#include "redisstringoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisStringOperations::RedisStringOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisStringOperations::~RedisStringOperations()
{
}

bool RedisStringOperations::set(const QString &key, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->set(key.toStdString(), value.toStdString());
        qDebug() << "SET" << key << "=" << value;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "SET error:" << e.what();
        return false;
    }
}

QString RedisStringOperations::get(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return QString();
    }

    try {
        auto value = connection_->redis()->get(key.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "GET" << key << "=" << result;
            return result;
        }
        qDebug() << "GET" << key << "= (null)";
        return QString();
    } catch (const std::exception &e) {
        qCritical() << "GET error:" << e.what();
        return QString();
    }
}
