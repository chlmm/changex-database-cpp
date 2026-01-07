#include "redissortedsetoperations.h"
#include "redisconnection.h"
#include <QDebug>

RedisSortedSetOperations::RedisSortedSetOperations(RedisConnection* connection)
    : connection_(connection)
{
}

RedisSortedSetOperations::~RedisSortedSetOperations()
{
}

bool RedisSortedSetOperations::zAdd(const QString &key, double score, const QString &member)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }

    try {
        connection_->redis()->zadd(key.toStdString(), member.toStdString(), score);
        qDebug() << "ZADD" << key << score << member;
        return true;
    } catch (const std::exception &e) {
        qCritical() << "ZADD error:" << e.what();
        return false;
    }
}

QVector<QString> RedisSortedSetOperations::zRange(const QString &key, int start, int stop)
{
    QVector<QString> result;
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return result;
    }

    try {
        std::vector<std::pair<std::string, double>> elements;
        connection_->redis()->zrange(key.toStdString(), start, stop, std::back_inserter(elements));
        for (const auto &element : elements) {
            result.append(QString::fromStdString(element.first));
        }
        qDebug() << "ZRANGE" << key << start << stop << "found" << result.size() << "elements";
    } catch (const std::exception &e) {
        qCritical() << "ZRANGE error:" << e.what();
    }

    return result;
}

double RedisSortedSetOperations::zScore(const QString &key, const QString &member)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return 0.0;
    }

    try {
        auto score = connection_->redis()->zscore(key.toStdString(), member.toStdString());
        if (score) {
            qDebug() << "ZSCORE" << key << member << "=" << *score;
            return *score;
        }
        qDebug() << "ZSCORE" << key << member << "= (null)";
        return 0.0;
    } catch (const std::exception &e) {
        qCritical() << "ZSCORE error:" << e.what();
        return 0.0;
    }
}

long long RedisSortedSetOperations::zRank(const QString &key, const QString &member)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return -1;
    }

    try {
        auto rank = connection_->redis()->zrank(key.toStdString(), member.toStdString());
        if (rank) {
            qDebug() << "ZRANK" << key << member << "=" << *rank;
            return *rank;
        }
        qDebug() << "ZRANK" << key << member << "= (null)";
        return -1;
    } catch (const std::exception &e) {
        qCritical() << "ZRANK error:" << e.what();
        return -1;
    }
}

long long RedisSortedSetOperations::zRevRank(const QString &key, const QString &member)
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return -1;
    }

    try {
        auto rank = connection_->redis()->zrevrank(key.toStdString(), member.toStdString());
        if (rank) {
            qDebug() << "ZREVRANK" << key << member << "=" << *rank;
            return *rank;
        }
        qDebug() << "ZREVRANK" << key << member << "= (null)";
        return -1;
    } catch (const std::exception &e) {
        qCritical() << "ZREVRANK error:" << e.what();
        return -1;
    }
}
