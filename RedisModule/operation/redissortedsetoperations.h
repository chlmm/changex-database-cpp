#ifndef REDISSORTEDSETOPERATIONS_H
#define REDISSORTEDSETOPERATIONS_H

#include <QString>
#include <QVector>
#include "../tool/redisoperationsbase.h"

class RedisSortedSetOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisSortedSetOperations对象,析构函数清理资源
    */
    explicit RedisSortedSetOperations(RedisConnection* connection);
    ~RedisSortedSetOperations();

    /**
    * @brief 有序集合操作
    *
    * 向有序集合中添加成员,获取有序集合指定范围内的成员
    * 获取有序集合中成员的分数,获取有序集合中成员的排名(升序/降序)
    */
    bool zAdd(const QString &key, double score, const QString &member);
    QVector<QString> zRange(const QString &key, int start, int stop);
    double zScore(const QString &key, const QString &member);
    long long zRank(const QString &key, const QString &member);
    long long zRevRank(const QString &key, const QString &member);
};

#endif // REDISSORTEDSETOPERATIONS_H
