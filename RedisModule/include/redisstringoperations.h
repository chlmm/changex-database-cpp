#ifndef REDISSTRINGOPERATIONS_H
#define REDISSTRINGOPERATIONS_H

#include <QString>

// Forward declaration
class RedisConnection;

class RedisStringOperations
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisStringOperations对象,析构函数清理资源
    */
    explicit RedisStringOperations(RedisConnection* connection);
    ~RedisStringOperations();

    /**
    * @brief 字符串操作
    *
    * 设置字符串键值对,获取字符串值
    */
    bool set(const QString &key, const QString &value);
    QString get(const QString &key);

private:
    RedisConnection* connection_;
};

#endif // REDISSTRINGOPERATIONS_H
