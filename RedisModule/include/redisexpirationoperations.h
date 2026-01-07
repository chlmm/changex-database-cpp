#ifndef REDISEXPIRATIONOPERATIONS_H
#define REDISEXPIRATIONOPERATIONS_H

#include <QString>

// Forward declaration
class RedisConnection;

class RedisExpirationOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisExpirationOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisExpirationOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisExpirationOperations();

    /**
     * @brief 设置键的过期时间(秒)
     * @param key 键名
     * @param seconds 过期秒数
     * @return 设置是否成功
     */
    bool expire(const QString &key, int seconds);

    /**
     * @brief 设置键的过期时间(时间戳)
     * @param key 键名
     * @param timestamp Unix时间戳
     * @return 设置是否成功
     */
    bool expireAt(const QString &key, qint64 timestamp);

    /**
     * @brief 获取键的剩余生存时间
     * @param key 键名
     * @return 剩余秒数
     */
    int ttl(const QString &key);

    /**
     * @brief 移除键的过期时间,使其永久存在
     * @param key 键名
     * @return 操作是否成功
     */
    bool persist(const QString &key);

private:
    RedisConnection* connection_;
};

#endif // REDISEXPIRATIONOPERATIONS_H
