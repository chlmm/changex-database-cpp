#ifndef REDISSETOPERATIONS_H
#define REDISSETOPERATIONS_H

#include <QString>
#include <QVector>

// Forward declaration
class RedisConnection;

class RedisSetOperations
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisSetOperations对象,析构函数清理资源
    */
    explicit RedisSetOperations(RedisConnection* connection);
    ~RedisSetOperations();

    /**
    * @brief 集合操作
    *
    * 向集合中添加成员,检查成员是否在集合中,从集合中移除成员,获取集合中所有成员,获取集合中成员数量
    * 获取多个集合的并集,获取多个集合的交集,获取多个集合的差集
    */
    bool sAdd(const QString &key, const QString &value);
    bool sIsMember(const QString &key, const QString &value);
    bool sRem(const QString &key, const QString &value);
    QVector<QString> sMembers(const QString &key);
    int sCard(const QString &key);
    QVector<QString> sUnion(const QVector<QString> &keys);
    QVector<QString> sInter(const QVector<QString> &keys);
    QVector<QString> sDiff(const QVector<QString> &keys);

private:
    RedisConnection* connection_;
};

#endif // REDISSETOPERATIONS_H
