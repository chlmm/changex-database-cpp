#ifndef REDISMANAGER_H
#define REDISMANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>

#include "tool/redisconnection.h"
#include "operation/redisstringoperations.h"
#include "operation/redisbytesoperations.h"
#include "operation/redishashoperations.h"
#include "operation/redislistoperations.h"
#include "operation/redissetoperations.h"
#include "operation/redissortedsetoperations.h"
#include "operation/redisexpirationoperations.h"
#include "operation/redistransactionoperations.h"
#include "operation/redisgenericoperations.h"

#include "tool/redismodule_export.h"

class REDISMODULESHARED_EXPORT RedisManager : public QObject
{
    Q_OBJECT

public:
    explicit RedisManager(QObject *parent = nullptr);
    ~RedisManager();

    /**
    * @brief 连接redis服务器
    *
    * 连接到指定主机和端口的Redis服务器, 断开服务器,是否连接到服务器
    */
    bool connectToServer(const QString &host = "127.0.0.1", int port = 6379);
    void disconnect();
    bool isConnected() const;

    /**
    * @brief 字符串操作
    *
    * 设置字符串键值对,获取字符串值
    */
    bool set(const QString &key, const QString &value);
    QString get(const QString &key);

    /**
    * @brief 字节流操作
    *
    * 设置字节流键值对(支持任意二进制数据如图片/音频/protobuf等),获取字节流值
    */
    bool bytesSet(const QString &key, const QByteArray &value);
    QByteArray bytesGet(const QString &key);
    bool bytesDel(const QString &key);
    bool bytesExists(const QString &key);
    bool bytesAppend(const QString &key, const QByteArray &value);
    int bytesSize(const QString &key);

    /**
    * @brief 哈希表操作
    *
    * 设置哈希表字段,获取哈希表字段值,获取哈希表中所有字段和值,删除哈希表字段,检查哈希表字段是否存在
    * 获取哈希表中所有字段名,获取哈希表中字段数量
    */
    bool hSet(const QString &key, const QString &field, const QString &value);
    QString hGet(const QString &key, const QString &field);
    QMap<QString, QString> hGetAll(const QString &key);
    bool hDel(const QString &key, const QString &field);
    bool hExists(const QString &key, const QString &field);
    QVector<QString> hKeys(const QString &key);
    int hLen(const QString &key);

    /**
    * @brief 列表操作
    *
    * 从列表左侧推入值,弹出值
    * 从列表右侧推入值,弹出值
    * 获取列表指定范围内的元素,获取列表长度,获取列表中指定索引的元素
    */
    bool lPush(const QString &key, const QString &value);
    QString lPop(const QString &key);
    bool rPush(const QString &key, const QString &value);
    QString rPop(const QString &key);
    QVector<QString> lRange(const QString &key, int start, int stop);
    int lLen(const QString &key);
    QString lIndex(const QString &key, int index);

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

    /**
    * @brief 键操作
    *
    * 删除键,检查键是否存在,查找符合模式的键
    */
    bool del(const QString &key);
    bool exists(const QString &key);
    QVector<QString> keys(const QString &pattern);

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

    /**
    * @brief 有序集合操作
    *
    * 向有序集合中添加成员,获取有序集合指定范围内的成员
    * 获取有序集合中成员的分数,获取有序集合中成员的排名(升序/降序)
    */
    bool zAdd(const QString &key, double score, const QString &member);
    QVector<QString> zRange(const QString &key, int start, int stop);
    double zScore(const QString &key, const QString &member);
    long long zRank(const QString &key, const QString &member);
    long long zRevRank(const QString &key, const QString &member);

    /**
    * @brief 事务操作
    *
    * 开启事务模式,执行事务中的所有命令
    * 取消事务模式放弃所有已排队命令,监视键
    */
    void multi();
    bool exec();
    void discard();
    bool watch(const QString &key);

private:
    RedisConnection connection_;
    RedisStringOperations stringOps_;
    RedisBytesOperations bytesOps_;
    RedisHashOperations hashOps_;
    RedisListOperations listOps_;
    RedisSetOperations setOps_;
    RedisSortedSetOperations sortedSetOps_;
    RedisExpirationOperations expirationOps_;
    RedisTransactionOperations transactionOps_;
    RedisGenericOperations genericOps_;
};

#endif // REDISMANAGER_H
