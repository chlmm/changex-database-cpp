#include "redishashoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisHashOperations::RedisHashOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisHashOperations::~RedisHashOperations()
{
}

bool RedisHashOperations::hSet(const QString &key, const QString &field, const QString &value)
{
    return execute([&]() {
        connection_->redis()->hset(key.toStdString(), field.toStdString(), value.toStdString());
        qDebug() << "HSET" << key << field << "=" << value;
        return true;
    }, "HSET", false);
}

QString RedisHashOperations::hGet(const QString &key, const QString &field)
{
    return execute([&]() {
        auto value = connection_->redis()->hget(key.toStdString(), field.toStdString());
        if (value) {
            QString result = QString::fromStdString(*value);
            qDebug() << "HGET" << key << field << "=" << result;
            return result;
        }
        qDebug() << "HGET" << key << field << "= (null)";
        return QString();
    }, "HGET", QString());
}

QMap<QString, QString> RedisHashOperations::hGetAll(const QString &key)
{
    return execute([&]() {
        std::unordered_map<std::string, std::string> hash;
        connection_->redis()->hgetall(key.toStdString(), std::inserter(hash, hash.begin()));
        QMap<QString, QString> result;
        for (const auto &pair : hash) {
            result.insert(QString::fromStdString(pair.first), QString::fromStdString(pair.second));
        }
        qDebug() << "HGETALL" << key << "找到" << result.size() << "个字段";
        return result;
    }, "HGETALL", QMap<QString, QString>());
}

bool RedisHashOperations::hDel(const QString &key, const QString &field)
{
    return execute([&]() {
        connection_->redis()->hdel(key.toStdString(), field.toStdString());
        qDebug() << "HDEL" << key << field;
        return true;
    }, "HDEL", false);
}

bool RedisHashOperations::hExists(const QString &key, const QString &field)
{
    return execute([&]() {
        bool result = connection_->redis()->hexists(key.toStdString(), field.toStdString());
        qDebug() << "HEXISTS" << key << field << "=" << result;
        return result;
    }, "HEXISTS", false);
}

QVector<QString> RedisHashOperations::hKeys(const QString &key)
{
    return execute([&]() {
        std::vector<std::string> keys;
        connection_->redis()->hkeys(key.toStdString(), std::back_inserter(keys));
        QVector<QString> result;
        for (const auto &k : keys) {
            result.append(QString::fromStdString(k));
        }
        qDebug() << "HKEYS" << key << "找到" << result.size() << "个键";
        return result;
    }, "HKEYS", QVector<QString>());
}

int RedisHashOperations::hLen(const QString &key)
{
    return execute([&]() {
        long long len = connection_->redis()->hlen(key.toStdString());
        qDebug() << "HLEN" << key << "=" << len;
        return static_cast<int>(len);
    }, "HLEN", 0);
}
