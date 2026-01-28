#ifndef REDISOPERATIONSBASE_H
#define REDISOPERATIONSBASE_H

#include <QString>
#include <QDebug>
#include <functional>

class RedisConnection;

class RedisOperationsBase
{
public:
    explicit RedisOperationsBase(RedisConnection* connection);
    virtual ~RedisOperationsBase();

protected:
    /**
     * @brief 检查连接是否有效
     * @return 连接有效返回 true
     */
    bool checkConnection() const;

    /**
     * @brief 执行 Redis 操作（有返回值版本）
     * @tparam Func 操作函数类型
     * @tparam Default 默认值类型
     * @param func 实际执行的操作
     * @param operation 操作名称（用于日志）
     * @param defaultValue 失败时的默认值
     * @return 操作结果或默认值
     */
    template<typename Func, typename Default>
    auto execute(Func&& func, const QString& operation, Default defaultValue) -> decltype(func())
    {
        if (!checkConnection()) {
            return defaultValue;
        }

        try {
            return func();
        } catch (const std::exception &e) {
            qCritical() << operation << "error:" << e.what();
            return defaultValue;
        }
    }

    /**
     * @brief 执行 Redis 操作（void 返回值版本）
     * @tparam Func 操作函数类型
     * @param func 实际执行的操作
     * @param operation 操作名称（用于日志）
     */
    template<typename Func>
    void executeVoid(Func&& func, const QString& operation)
    {
        if (!checkConnection()) {
            return;
        }

        try {
            func();
        } catch (const std::exception &e) {
            qCritical() << operation << "error:" << e.what();
        }
    }

    RedisConnection* connection_;
};

#endif // REDISOPERATIONSBASE_H
