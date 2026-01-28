#ifndef REDISBYTESOPERATIONS_H
#define REDISBYTESOPERATIONS_H

#include <QString>
#include <QByteArray>
#include "../tool/redisoperationsbase.h"

/**
 * @brief Redis字节流操作类
 *
 * 专门处理二进制数据的存储和读取，支持任意字节流如图片、音频、protobuf序列化数据等
 */
class RedisBytesOperations : public RedisOperationsBase
{
public:
    /**
    * @brief 构造函数和析构函数
    */
    explicit RedisBytesOperations(RedisConnection* connection);
    ~RedisBytesOperations();

    /**
    * @brief 字节流操作
    *
    * 设置字节流键值对,获取字节流值,删除字节流键
    */
    bool set(const QString &key, const QByteArray &value);
    QByteArray get(const QString &key);
    bool del(const QString &key);
    bool exists(const QString &key);

    /**
    * @brief 字节流追加操作
    *
    * 在已有字节流后追加数据
    */
    bool append(const QString &key, const QByteArray &value);

    /**
    * @brief 获取字节流长度
    */
    int size(const QString &key);
};

#endif // REDISBYTESOPERATIONS_H
