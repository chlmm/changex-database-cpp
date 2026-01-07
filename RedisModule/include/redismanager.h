#ifndef REDISMANAGER_H
#define REDISMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>

#include "redisconnection.h"
#include "redisstringoperations.h"
#include "redishashoperations.h"
#include "redislistoperations.h"
#include "redissetoperations.h"
#include "redissortedsetoperations.h"
#include "redisexpirationoperations.h"
#include "redistransactionoperations.h"
#include "redisgenericoperations.h"

#include "redismodule_export.h"

class REDISMODULESHARED_EXPORT RedisManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数,初始化RedisManager对象
     * @param parent 父对象指针,默认为nullptr
     */
    explicit RedisManager(QObject *parent = nullptr);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisManager();

    // Connection management
    /**
     * @brief 连接到Redis服务器
     * @param host 服务器主机地址,默认为"127.0.0.1"
     * @param port 服务器端口号,默认为6379
     * @return 连接是否成功
     */
    bool connectToServer(const QString &host = "127.0.0.1", int port = 6379);

    /**
     * @brief 断开与Redis服务器的连接
     */
    void disconnect();

    /**
     * @brief 检查是否已连接到Redis服务器
     * @note 此函数不修改对象状态
     * @return 是否已连接
     */
    bool isConnected() const;

    // String operations
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

    // Hash operations
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

    // List operations
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

    // Set operations
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

    // Generic operations
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

    // Expiration operations
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

    // Sorted Set operations
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

    // Transaction operations
    /**
     * @brief 开启事务模式
     */
    void multi();

    /**
     * @brief 执行事务中的所有命令
     * @return 执行是否成功
     */
    bool exec();

    /**
     * @brief 取消事务模式,放弃所有已排队命令
     */
    void discard();

    /**
     * @brief 监视键,如果在事务执行前被修改则事务失败
     * @param key 要监视的键名
     * @return 监视是否成功
     */
    bool watch(const QString &key);

private:
    RedisConnection connection_;
    RedisStringOperations stringOps_;
    RedisHashOperations hashOps_;
    RedisListOperations listOps_;
    RedisSetOperations setOps_;
    RedisSortedSetOperations sortedSetOps_;
    RedisExpirationOperations expirationOps_;
    RedisTransactionOperations transactionOps_;
    RedisGenericOperations genericOps_;
};

#endif // REDISMANAGER_H
