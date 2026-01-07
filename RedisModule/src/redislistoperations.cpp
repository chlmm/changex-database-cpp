#include "redislistoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisListOperations::RedisListOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisListOperations::~RedisListOperations()
{
}

bool RedisListOperations::lPush(const QString &key, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->lpush(key.toStdString(), value.toStdString());
        qDebug() << "LPUSH" << key << value;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "LPUSH error:" << e.what();
        return false;
    }
}

QString RedisListOperations::lPop(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return QString();
    }

    try {
        auto value = connection_->redis()->lpop(key.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "LPOP" << key << "=" << result;
            return result;
        }
        qDebug() << "LPOP" << key << "= (null)";
        return QString();
    } catch (const std::exception &e) {
        qCritical() << "LPOP error:" << e.what();
        return QString();
    }
}

bool RedisListOperations::rPush(const QString &key, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->rpush(key.toStdString(), value.toStdString());
        qDebug() << "RPUSH" << key << value;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "RPUSH error:" << e.what();
        return false;
    }
}

QString RedisListOperations::rPop(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return QString();
    }

    try {
        auto value = connection_->redis()->rpop(key.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "RPOP" << key << "=" << result;
            return result;
        }
        qDebug() << "RPOP" << key << "= (null)";
        return QString();
    } catch (const std::exception &e) {
        qCritical() << "RPOP error:" << e.what();
        return QString();
    }
}

QVector<QString> RedisListOperations::lRange(const QString &key, int start, int stop)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::string> values;
        connection_->redis()->lrange(key.toStdString(), start, stop, std::back_inserter(values));
        for (const auto &value : values) {
            result.append(QString::fromStdString(value));
        }
        qDebug() << "LRANGE" << key << start << stop << "found" << result.size() << "elements";
    } catch (const std::exception &e) {
        qCritical() << "LRANGE error:" << e.what();
    }

    return result;
}

int RedisListOperations::lLen(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return 0;
    }

    try {
        long long len = connection_->redis()->llen(key.toStdString());
        qDebug() << "LLEN" << key << "=" << len;
        return static_cast<int>(len);
    } catch (const std::exception &e) {
        qCritical() << "LLEN error:" << e.what();
        return 0;
    }
}

QString RedisListOperations::lIndex(const QString &key, int index)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return QString();
    }

    try {
        auto value = connection_->redis()->lindex(key.toStdString(), index);
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "LINDEX" << key << index << "=" << result;
            return result;
        }
        qDebug() << "LINDEX" << key << index << "= (null)";
        return QString();
    } catch (const std::exception &e) {
        qCritical() << "LINDEX error:" << e.what();
        return QString();
    }
}
