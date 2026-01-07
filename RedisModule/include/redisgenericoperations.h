#ifndef REDISGENERICOPERATIONS_H
#define REDISGENERICOPERATIONS_H

#include <QString>
#include <QVector>

// Forward declaration
class RedisConnection;

class RedisGenericOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisGenericOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisGenericOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisGenericOperations();

    /**
     * @brief 删除键
     * @param key 键名
     * @return 删除是否成功
     */
    bool del(const QString &key);

    /**
     * @brief 检查键是否存在
     * @param key 键名
     * @return 键是否存在
     */
    bool exists(const QString &key);

    /**
     * @brief 查找符合模式的键
     * @param pattern 查找模式
     * @return 匹配的键列表
     */
    QVector<QString> keys(const QString &pattern);

private:
    RedisConnection* connection_;
};

#endif // REDISGENERICOPERATIONS_H
