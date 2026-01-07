#ifndef REDISSORTEDSETOPERATIONS_H
#define REDISSORTEDSETOPERATIONS_H

#include <QString>
#include <QVector>

// Forward declaration
class RedisConnection;

class RedisSortedSetOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisSortedSetOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisSortedSetOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisSortedSetOperations();

    /**
     * @brief 向有序集合中添加成员
     * @param key 有序集合键名
     * @param score 分数值
     * @param member 成员值
     * @return 添加是否成功
     */
    bool zAdd(const QString &key, double score, const QString &member);

    /**
     * @brief 获取有序集合指定范围内的成员
     * @param key 有序集合键名
     * @param start 起始索引
     * @param stop 结束索引
     * @return 指定范围内的成员列表
     */
    QVector<QString> zRange(const QString &key, int start, int stop);

    /**
     * @brief 获取有序集合中成员的分数
     * @param key 有序集合键名
     * @param member 成员值
     * @return 成员的分数值
     */
    double zScore(const QString &key, const QString &member);

    /**
     * @brief 获取有序集合中成员的排名(升序)
     * @param key 有序集合键名
     * @param member 成员值
     * @return 成员的排名
     */
    long long zRank(const QString &key, const QString &member);

    /**
     * @brief 获取有序集合中成员的排名(降序)
     * @param key 有序集合键名
     * @param member 成员值
     * @return 成员的排名
     */
    long long zRevRank(const QString &key, const QString &member);

private:
    RedisConnection* connection_;
};

#endif // REDISSORTEDSETOPERATIONS_H
