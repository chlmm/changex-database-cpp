#include "redisoperationsbase.h"
#include "redisconnection.h"
#include <QDebug>

RedisOperationsBase::RedisOperationsBase(RedisConnection* connection)
    : connection_(connection)
{
}

RedisOperationsBase::~RedisOperationsBase()
{
}

bool RedisOperationsBase::checkConnection() const
{
    if (!connection_ || !connection_->isConnected() || !connection_->redis()) {
        qWarning() << "Not connected to Redis";
        return false;
    }
    return true;
}
