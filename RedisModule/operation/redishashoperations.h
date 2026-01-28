#ifndef REDISHASHOPERATIONS_H
#define REDISHASHOPERATIONS_H

#include <QString>
#include <QMap>
#include <QVector>
#include "../tool/redisoperationsbase.h"

class RedisHashOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisHashOperations对象,析构函数清理资源
    */
    explicit RedisHashOperations(RedisConnection* connection);
    ~RedisHashOperations();

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
};

#endif // REDISHASHOPERATIONS_H
