#include "redissetoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisSetOperations::RedisSetOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisSetOperations::~RedisSetOperations()
{
}

bool RedisSetOperations::sAdd(const QString &key, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->sadd(key.toStdString(), value.toStdString());
        qDebug() << "SADD" << key << value;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "SADD error:" << e.what();
        return false;
    }
}

bool RedisSetOperations::sIsMember(const QString &key, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        bool result = connection_->redis()->sismember(key.toStdString(), value.toStdString());
        qDebug() << "SISMEMBER" << key << value << "=" << result;
        return result;
    } catch (const std::exception &e) {
        qCritical() << "SISMEMBER error:" << e.what();
        return false;
    }
}

bool RedisSetOperations::sRem(const QString &key, const QString &value)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->srem(key.toStdString(), value.toStdString());
        qDebug() << "SREM" << key << value;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "SREM error:" << e.what();
        return false;
    }
}

QVector<QString> RedisSetOperations::sMembers(const QString &key)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::unordered_set<std::string> members;
        connection_->redis()->smembers(key.toStdString(), std::inserter(members, members.begin()));
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SMEMBERS" << key << "found" << result.size() << "members";
    } catch (const std::exception &e) {
        qCritical() << "SMEMBERS error:" << e.what();
    }

    return result;
}

int RedisSetOperations::sCard(const QString &key)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return 0;
    }

    try {
        long long count = connection_->redis()->scard(key.toStdString());
        qDebug() << "SCARD" << key << "=" << count;
        return static_cast<int>(count);
    } catch (const std::exception &e) {
        qCritical() << "SCARD error:" << e.what();
        return 0;
    }
}

QVector<QString> RedisSetOperations::sUnion(const QVector<QString> &keys)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::string> keysVec;
        for (const auto &key : keys) {
            keysVec.push_back(key.toStdString());
        }
        std::unordered_set<std::string> members;
        connection_->redis()->sunion(keysVec.begin(), keysVec.end(), std::inserter(members, members.begin()));
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SUNION" << "found" << result.size() << "members";
    } catch (const std::exception &e) {
        qCritical() << "SUNION error:" << e.what();
    }

    return result;
}

QVector<QString> RedisSetOperations::sInter(const QVector<QString> &keys)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::string> keysVec;
        for (const auto &key : keys) {
            keysVec.push_back(key.toStdString());
        }
        std::unordered_set<std::string> members;
        connection_->redis()->sinter(keysVec.begin(), keysVec.end(), std::inserter(members, members.begin()));
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SINTER" << "found" << result.size() << "members";
    } catch (const std::exception &e) {
        qCritical() << "SINTER error:" << e.what();
    }

    return result;
}

QVector<QString> RedisSetOperations::sDiff(const QVector<QString> &keys)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::string> keysVec;
        for (const auto &key : keys) {
            keysVec.push_back(key.toStdString());
        }
        std::unordered_set<std::string> members;
        connection_->redis()->sdiff(keysVec.begin(), keysVec.end(), std::inserter(members, members.begin()));
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SDIFF" << "found" << result.size() << "members";
    } catch (const std::exception &e) {
        qCritical() << "SDIFF error:" << e.what();
    }

    return result;
}
