#ifndef REDISGENERICOPERATIONS_H
#define REDISGENERICOPERATIONS_H

#include <QString>
#include <QVector>
#include "../tool/redisoperationsbase.h"

class RedisGenericOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisGenericOperations对象,析构函数清理资源
    */
    explicit RedisGenericOperations(RedisConnection* connection);
    ~RedisGenericOperations();

    /**
    * @brief 键操作
    *
    * 删除键,检查键是否存在,查找符合模式的键
    */
    bool del(const QString &key);
    bool exists(const QString &key);
    QVector<QString> keys(const QString &pattern);
};

#endif // REDISGENERICOPERATIONS_H
