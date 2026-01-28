#include "redissetoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisSetOperations::RedisSetOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisSetOperations::~RedisSetOperations()
{
}

bool RedisSetOperations::sAdd(const QString &key, const QString &value)
{
    return execute([&]() {
        connection_->redis()->sadd(key.toStdString(), value.toStdString());
        qDebug() << "SADD" << key << value;
        return true;
    }, "SADD", false);
}

bool RedisSetOperations::sIsMember(const QString &key, const QString &value)
{
    return execute([&]() {
        bool result = connection_->redis()->sismember(key.toStdString(), value.toStdString());
        qDebug() << "SISMEMBER" << key << value << "=" << result;
        return result;
    }, "SISMEMBER", false);
}

bool RedisSetOperations::sRem(const QString &key, const QString &value)
{
    return execute([&]() {
        connection_->redis()->srem(key.toStdString(), value.toStdString());
        qDebug() << "SREM" << key << value;
        return true;
    }, "SREM", false);
}

QVector<QString> RedisSetOperations::sMembers(const QString &key)
{
    return execute([&]() {
        std::unordered_set<std::string> members;
        connection_->redis()->smembers(key.toStdString(), std::inserter(members, members.begin()));
        QVector<QString> result;
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SMEMBERS" << key << "找到" << result.size() << "个成员";
        return result;
    }, "SMEMBERS", QVector<QString>());
}

int RedisSetOperations::sCard(const QString &key)
{
    return execute([&]() {
        long long count = connection_->redis()->scard(key.toStdString());
        qDebug() << "SCARD" << key << "=" << count;
        return static_cast<int>(count);
    }, "SCARD", 0);
}

QVector<QString> RedisSetOperations::sUnion(const QVector<QString> &keys)
{
    return execute([&]() {
        std::vector<std::string> keysVec;
        for (const auto &key : keys) {
            keysVec.push_back(key.toStdString());
        }
        std::unordered_set<std::string> members;
        connection_->redis()->sunion(keysVec.begin(), keysVec.end(), std::inserter(members, members.begin()));
        QVector<QString> result;
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SUNION" << "找到" << result.size() << "个成员";
        return result;
    }, "SUNION", QVector<QString>());
}

QVector<QString> RedisSetOperations::sInter(const QVector<QString> &keys)
{
    return execute([&]() {
        std::vector<std::string> keysVec;
        for (const auto &key : keys) {
            keysVec.push_back(key.toStdString());
        }
        std::unordered_set<std::string> members;
        connection_->redis()->sinter(keysVec.begin(), keysVec.end(), std::inserter(members, members.begin()));
        QVector<QString> result;
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SINTER" << "找到" << result.size() << "个成员";
        return result;
    }, "SINTER", QVector<QString>());
}

QVector<QString> RedisSetOperations::sDiff(const QVector<QString> &keys)
{
    return execute([&]() {
        std::vector<std::string> keysVec;
        for (const auto &key : keys) {
            keysVec.push_back(key.toStdString());
        }
        std::unordered_set<std::string> members;
        connection_->redis()->sdiff(keysVec.begin(), keysVec.end(), std::inserter(members, members.begin()));
        QVector<QString> result;
        for (const auto &member : members) {
            result.append(QString::fromStdString(member));
        }
        qDebug() << "SDIFF" << "找到" << result.size() << "个成员";
        return result;
    }, "SDIFF", QVector<QString>());
}
