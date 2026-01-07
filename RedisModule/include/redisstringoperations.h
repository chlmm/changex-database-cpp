#ifndef REDISSTRINGOPERATIONS_H
#define REDISSTRINGOPERATIONS_H

#include <QString>

// Forward declaration
class RedisConnection;

class RedisStringOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisStringOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisStringOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisStringOperations();

    /**
     * @brief 设置字符串键值对
     * @param key 键名
     * @param value 值
     * @return 设置是否成功
     */
    bool set(const QString &key, const QString &value);

    /**
     * @brief 获取字符串值
     * @param key 键名
     * @return 键对应的值
     */
    QString get(const QString &key);

private:
    RedisConnection* connection_;
};

#endif // REDISSTRINGOPERATIONS_H
