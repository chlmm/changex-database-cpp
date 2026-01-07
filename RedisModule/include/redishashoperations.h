#ifndef REDISHASHOPERATIONS_H
#define REDISHASHOPERATIONS_H

#include <QString>
#include <QMap>
#include <QVector>

// Forward declaration
class RedisConnection;

class RedisHashOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisHashOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisHashOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisHashOperations();

    /**
     * @brief 设置哈希表字段
     * @param key 哈希表键名
     * @param field 字段名
     * @param value 字段值
     * @return 设置是否成功
     */
    bool hSet(const QString &key, const QString &field, const QString &value);

    /**
     * @brief 获取哈希表字段值
     * @param key 哈希表键名
     * @param field 字段名
     * @return 字段对应的值
     */
    QString hGet(const QString &key, const QString &field);

    /**
     * @brief 获取哈希表中所有字段和值
     * @param key 哈希表键名
     * @return 包含所有字段和值的映射
     */
    QMap<QString, QString> hGetAll(const QString &key);

    /**
     * @brief 删除哈希表字段
     * @param key 哈希表键名
     * @param field 字段名
     * @return 删除是否成功
     */
    bool hDel(const QString &key, const QString &field);

    /**
     * @brief 检查哈希表字段是否存在
     * @param key 哈希表键名
     * @param field 字段名
     * @return 字段是否存在
     */
    bool hExists(const QString &key, const QString &field);

    /**
     * @brief 获取哈希表中所有字段名
     * @param key 哈希表键名
     * @return 所有字段名的列表
     */
    QVector<QString> hKeys(const QString &key);

    /**
     * @brief 获取哈希表中字段数量
     * @param key 哈希表键名
     * @return 字段数量
     */
    int hLen(const QString &key);

private:
    RedisConnection* connection_;
};

#endif // REDISHASHOPERATIONS_H
