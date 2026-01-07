#include "redishashoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisHashOperations::RedisHashOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisHashOperations::~RedisHashOperations()
{
}

bool RedisHashOperations::hSet(const QString &key, const QString &field, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->hset(key.toStdString(), field.toStdString(), value.toStdString());
        qDebug() << "HSET" << key << field << "=" << value;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "HSET error:" << e.what();
        return false;
    }
}

QString RedisHashOperations::hGet(const QString &key, const QString &field)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return QString();
    }

    try {
        auto value = connection_->redis()->hget(key.toStdString(), field.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "HGET" << key << field << "=" << result;
            return result;
        }
        qDebug() << "HGET" << key << field << "= (null)";
        return QString();
    } catch (const std::exception &e) {
        qCritical() << "HGET error:" << e.what();
        return QString();
    }
}

QMap<QString, QString> RedisHashOperations::hGetAll(const QString &key)
{
    QMap<QString, QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::unordered_map<std::string, std::string> hash;
        connection_->redis()->hgetall(key.toStdString(), std::inserter(hash, hash.begin()));
        for (const auto &pair : hash) {
            result.insert(QString::fromStdString(pair.first), QString::fromStdString(pair.second));
        }
        qDebug() << "HGETALL" << key << "found" << result.size() << "fields";
    } catch (const std::exception &e) {
        qCritical() << "HGETALL error:" << e.what();
    }

    return result;
}

bool RedisHashOperations::hDel(const QString &key, const QString &field)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->hdel(key.toStdString(), field.toStdString());
        qDebug() << "HDEL" << key << field;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "HDEL error:" << e.what();
        return false;
    }
}

bool RedisHashOperations::hExists(const QString &key, const QString &field)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        bool result = connection_->redis()->hexists(key.toStdString(), field.toStdString());
        qDebug() << "HEXISTS" << key << field << "=" << result;
        return result;
    } catch (const std::exception &e) {
        qCritical() << "HEXISTS error:" << e.what();
        return false;
    }
}

QVector<QString> RedisHashOperations::hKeys(const QString &key)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::string> keys;
        connection_->redis()->hkeys(key.toStdString(), std::back_inserter(keys));
        for (const auto &k : keys) {
            result.append(QString::fromStdString(k));
        }
        qDebug() << "HKEYS" << key << "found" << result.size() << "keys";
    } catch (const std::exception &e) {
        qCritical() << "HKEYS error:" << e.what();
    }

    return result;
}

int RedisHashOperations::hLen(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return 0;
    }

    try {
        long long len = connection_->redis()->hlen(key.toStdString());
        qDebug() << "HLEN" << key << "=" << len;
        return static_cast<int>(len);
    } catch (const std::exception &e) {
        qCritical() << "HLEN error:" << e.what();
        return 0;
    }
}
