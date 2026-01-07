#ifndef REDISLISTOPERATIONS_H
#define REDISLISTOPERATIONS_H

#include <QString>
#include <QVector>

// Forward declaration
class RedisConnection;

class RedisListOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisListOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisListOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisListOperations();

    /**
     * @brief 从列表左侧推入值
     * @param key 列表键名
     * @param value 要推入的值
     * @return 操作是否成功
     */
    bool lPush(const QString &key, const QString &value);

    /**
     * @brief 从列表左侧弹出值
     * @param key 列表键名
     * @return 弹出的值
     */
    QString lPop(const QString &key);

    /**
     * @brief 从列表右侧推入值
     * @param key 列表键名
     * @param value 要推入的值
     * @return 操作是否成功
     */
    bool rPush(const QString &key, const QString &value);

    /**
     * @brief 从列表右侧弹出值
     * @param key 列表键名
     * @return 弹出的值
     */
    QString rPop(const QString &key);

    /**
     * @brief 获取列表指定范围内的元素
     * @param key 列表键名
     * @param start 起始索引
     * @param stop 结束索引
     * @return 指定范围内的元素列表
     */
    QVector<QString> lRange(const QString &key, int start, int stop);

    /**
     * @brief 获取列表长度
     * @param key 列表键名
     * @return 列表长度
     */
    int lLen(const QString &key);

    /**
     * @brief 获取列表中指定索引的元素
     * @param key 列表键名
     * @param index 索引位置
     * @return 指定索引的元素
     */
    QString lIndex(const QString &key, int index);

private:
    RedisConnection* connection_;
};

#endif // REDISLISTOPERATIONS_H
