#include "redisbytesoperations.h"
#include "../tool/redisconnection.h"
#include <QDebug>

RedisBytesOperations::RedisBytesOperations(RedisConnection* connection)
    : RedisOperationsBase(connection)
{
}

RedisBytesOperations::~RedisBytesOperations()
{
}

bool RedisBytesOperations::set(const QString &key, const QByteArray &value)
{
    return execute([&]() {
        connection_->redis()->set(key.toStdString(), 
            std::string(value.constData(), value.size()));
        qDebug() << "BYTES_SET [设置字节流]" << key << "size=" << value.size();
        return true;
    }, "BYTES_SET", false);
}

QByteArray RedisBytesOperations::get(const QString &key)
{
    return execute([&]() {
        auto value = connection_->redis()->get(key.toStdString());
        if (value) {
            QByteArray result(value->data(), static_cast<int>(value->size()));
            qDebug() << "BYTES_GET [获取字节流]" << key << "size=" << result.size();
            return result;
        }
        qDebug() << "BYTES_GET [获取字节流]" << key << "= (null)";
        return QByteArray();
    }, "BYTES_GET", QByteArray());
}

bool RedisBytesOperations::del(const QString &key)
{
    return execute([&]() {
        connection_->redis()->del(key.toStdString());
        qDebug() << "BYTES_DEL [删除字节流]" << key;
        return true;
    }, "BYTES_DEL", false);
}

bool RedisBytesOperations::exists(const QString &key)
{
    return execute([&]() {
        bool result = connection_->redis()->exists(key.toStdString());
        qDebug() << "BYTES_EXISTS [检查字节流]" << key << "=" << result;
        return result;
    }, "BYTES_EXISTS", false);
}

bool RedisBytesOperations::append(const QString &key, const QByteArray &value)
{
    return execute([&]() {
        connection_->redis()->append(key.toStdString(),
            std::string(value.constData(), value.size()));
        qDebug() << "BYTES_APPEND [追加字节流]" << key << "size=" << value.size();
        return true;
    }, "BYTES_APPEND", false);
}

int RedisBytesOperations::size(const QString &key)
{
    return execute([&]() {
        long long len = connection_->redis()->strlen(key.toStdString());
        qDebug() << "BYTES_SIZE [字节流大小]" << key << "=" << len;
        return static_cast<int>(len);
    }, "BYTES_SIZE", 0);
}
