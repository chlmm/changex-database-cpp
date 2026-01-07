#ifndef REDISTRANSACTIONOPERATIONS_H
#define REDISTRANSACTIONOPERATIONS_H

#include <QString>

// Forward declaration
class RedisConnection;

class RedisTransactionOperations
{
public:
    /**
     * @brief 构造函数,初始化RedisTransactionOperations对象
     * @param connection Redis连接对象指针
     */
    explicit RedisTransactionOperations(RedisConnection* connection);

    /**
     * @brief 析构函数,清理资源
     */
    ~RedisTransactionOperations();

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
    RedisConnection* connection_;
};

#endif // REDISTRANSACTIONOPERATIONS_H
