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
     * @brief 构造函数,初始化RedisSetOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisSetOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisSetOperations();

    /**
     * @brief 向集合中添加成员
     * @param key 集合键名
     * @param value 成员值
     * @return 添加是否成功
     */
    bool sAdd(const QString &key, const QString &value);

    /**
     * @brief 检查成员是否在集合中
     * @param key 集合键名
     * @param value 成员值
     * @return 成员是否存在
     */
    bool sIsMember(const QString &key, const QString &value);

    /**
     * @brief 从集合中移除成员
     * @param key 集合键名
     * @param value 成员值
     * @return 移除是否成功
     */
    bool sRem(const QString &key, const QString &value);

    /**
     * @brief 获取集合中所有成员
     * @param key 集合键名
     * @return 所有成员的列表
     */
    QVector<QString> sMembers(const QString &key);

    /**
     * @brief 获取集合中成员数量
     * @param key 集合键名
     * @return 成员数量
     */
    int sCard(const QString &key);

    /**
     * @brief 获取多个集合的并集
     * @param keys 集合键名列表
     * @return 并集结果
     */
    QVector<QString> sUnion(const QVector<QString> &keys);

    /**
     * @brief 获取多个集合的交集
     * @param keys 集合键名列表
     * @return 交集结果
     */
    QVector<QString> sInter(const QVector<QString> &keys);

    /**
     * @brief 获取多个集合的差集
     * @param keys 集合键名列表
     * @return 差集结果
     */
    QVector<QString> sDiff(const QVector<QString> &keys);

private:
    RedisConnection* connection_;
};

#endif // REDISSETOPERATIONS_H
