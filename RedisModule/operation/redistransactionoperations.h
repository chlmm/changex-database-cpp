#ifndef REDISTRANSACTIONOPERATIONS_H
#define REDISTRANSACTIONOPERATIONS_H

#include <QString>
#include "../tool/redisoperationsbase.h"

class RedisTransactionOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    *
    * 构造函数初始化RedisTransactionOperations对象,析构函数清理资源
    */
    explicit RedisTransactionOperations(RedisConnection* connection);
    ~RedisTransactionOperations();

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
};

#endif // REDISTRANSACTIONOPERATIONS_H
