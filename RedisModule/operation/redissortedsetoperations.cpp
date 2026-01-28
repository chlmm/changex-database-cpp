#include "redissortedsetoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisSortedSetOperations::RedisSortedSetOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisSortedSetOperations::~RedisSortedSetOperations()
{
}

bool RedisSortedSetOperations::zAdd(const QString &key, double score, const QString &member)
{
    return execute([&]() {
        connection_->redis()->zadd(key.toStdString(), member.toStdString(), score);
        qDebug() << "ZADD" << key << score << member;
        return true;
    }, "ZADD", false);
}

QVector<QString> RedisSortedSetOperations::zRange(const QString &key, int start, int stop)
{
    return execute([&]() {
        std::vector<std::pair<std::string, double>> elements;
        connection_->redis()->zrange(key.toStdString(), start, stop, std::back_inserter(elements));
        QVector<QString> result;
        for (const auto &element : elements) {
            result.append(QString::fromStdString(element.first));
        }
        qDebug() << "ZRANGE" << key << start << stop << "找到" << result.size() << "个元素";
        return result;
    }, "ZRANGE", QVector<QString>());
}

double RedisSortedSetOperations::zScore(const QString &key, const QString &member)
{
    return execute([&]() {
        auto score = connection_->redis()->zscore(key.toStdString(), member.toStdString());
        if (score) {
            qDebug() << "ZSCORE" << key << member << "=" << *score;
            return *score;
        }
        qDebug() << "ZSCORE" << key << member << "= (null)";
        return 0.0;
    }, "ZSCORE", 0.0);
}

long long RedisSortedSetOperations::zRank(const QString &key, const QString &member)
{
    return execute([&]() -> long long {
        auto rank = connection_->redis()->zrank(key.toStdString(), member.toStdString());
        if (rank) {
            qDebug() << "ZRANK" << key << member << "=" << *rank;
            return *rank;
        }
        qDebug() << "ZRANK" << key << member << "= (null)";
        return -1;
    }, "ZRANK", -1);
}

long long RedisSortedSetOperations::zRevRank(const QString &key, const QString &member)
{
    return execute([&]() -> long long {
        auto rank = connection_->redis()->zrevrank(key.toStdString(), member.toStdString());
        if (rank) {
            qDebug() << "ZREVRANK" << key << member << "=" << *rank;
            return *rank;
        }
        qDebug() << "ZREVRANK" << key << member << "= (null)";
        return -1;
    }, "ZREVRANK", -1);
}
