#ifndef REDISEXPIRATIONOPERATIONS_H
#define REDISEXPIRATIONOPERATIONS_H

#include <QString>
#include "../tool/redisoperationsbase.h"

class RedisExpirationOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisExpirationOperations对象,析构函数清理资源
    */
    explicit RedisExpirationOperations(RedisConnection* connection);
    ~RedisExpirationOperations();

    /**
    * @brief 过期操作
    *
    * 设置键的过期时间(秒),设置键的过期时间(时间戳)
    * 获取键的剩余生存时间,移除键的过期时间使其永久存在
    */
    bool expire(const QString &key, int seconds);
    bool expireAt(const QString &key, qint64 timestamp);
    int ttl(const QString &key);
    bool persist(const QString &key);
};

#endif // REDISEXPIRATIONOPERATIONS_H
