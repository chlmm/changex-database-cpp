#ifndef REDISLISTOPERATIONS_H
#define REDISLISTOPERATIONS_H

#include <QString>
#include <QVector>
#include "../tool/redisoperationsbase.h"

class RedisListOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisListOperations对象,析构函数清理资源
    */
    explicit RedisListOperations(RedisConnection* connection);
    ~RedisListOperations();

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
};

#endif // REDISLISTOPERATIONS_H
