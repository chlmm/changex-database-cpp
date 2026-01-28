#include "redismanager.h"

RedisManager::RedisManager(QObject *parent)
    : QObject(parent)
    , connection_()
    , stringOps_(&connection_)
    , bytesOps_(&connection_)
    , hashOps_(&connection_)
    , listOps_(&connection_)
    , setOps_(&connection_)
    , sortedSetOps_(&connection_)
    , expirationOps_(&connection_)
    , transactionOps_(&connection_)
    , genericOps_(&connection_)
{
}

RedisManager::~RedisManager()
{
    disconnect();
}

bool RedisManager::connectToServer(const QString &host, int port)
{
    return connection_.connectToServer(host, port);
}

void RedisManager::disconnect()
{
    connection_.disconnect();
}

bool RedisManager::isConnected() const
{
    return connection_.isConnected();
}

// String operations
bool RedisManager::set(const QString &key, const QString &value)
{
    return stringOps_.set(key, value);
}

QString RedisManager::get(const QString &key)
{
    return stringOps_.get(key);
}

// Bytes operations
bool RedisManager::bytesSet(const QString &key, const QByteArray &value)
{
    return bytesOps_.set(key, value);
}

QByteArray RedisManager::bytesGet(const QString &key)
{
    return bytesOps_.get(key);
}

bool RedisManager::bytesDel(const QString &key)
{
    return bytesOps_.del(key);
}

bool RedisManager::bytesExists(const QString &key)
{
    return bytesOps_.exists(key);
}

bool RedisManager::bytesAppend(const QString &key, const QByteArray &value)
{
    return bytesOps_.append(key, value);
}

int RedisManager::bytesSize(const QString &key)
{
    return bytesOps_.size(key);
}

// Hash operations
bool RedisManager::hSet(const QString &key, const QString &field, const QString &value)
{
    return hashOps_.hSet(key, field, value);
}

QString RedisManager::hGet(const QString &key, const QString &field)
{
    return hashOps_.hGet(key, field);
}

QMap<QString, QString> RedisManager::hGetAll(const QString &key)
{
    return hashOps_.hGetAll(key);
}

bool RedisManager::hDel(const QString &key, const QString &field)
{
    return hashOps_.hDel(key, field);
}

bool RedisManager::hExists(const QString &key, const QString &field)
{
    return hashOps_.hExists(key, field);
}

QVector<QString> RedisManager::hKeys(const QString &key)
{
    return hashOps_.hKeys(key);
}

int RedisManager::hLen(const QString &key)
{
    return hashOps_.hLen(key);
}

// List operations
bool RedisManager::lPush(const QString &key, const QString &value)
{
    return listOps_.lPush(key, value);
}

QString RedisManager::lPop(const QString &key)
{
    return listOps_.lPop(key);
}

bool RedisManager::rPush(const QString &key, const QString &value)
{
    return listOps_.rPush(key, value);
}

QString RedisManager::rPop(const QString &key)
{
    return listOps_.rPop(key);
}

QVector<QString> RedisManager::lRange(const QString &key, int start, int stop)
{
    return listOps_.lRange(key, start, stop);
}

int RedisManager::lLen(const QString &key)
{
    return listOps_.lLen(key);
}

QString RedisManager::lIndex(const QString &key, int index)
{
    return listOps_.lIndex(key, index);
}

// Set operations
bool RedisManager::sAdd(const QString &key, const QString &value)
{
    return setOps_.sAdd(key, value);
}

bool RedisManager::sIsMember(const QString &key, const QString &value)
{
    return setOps_.sIsMember(key, value);
}

bool RedisManager::sRem(const QString &key, const QString &value)
{
    return setOps_.sRem(key, value);
}

QVector<QString> RedisManager::sMembers(const QString &key)
{
    return setOps_.sMembers(key);
}

int RedisManager::sCard(const QString &key)
{
    return setOps_.sCard(key);
}

QVector<QString> RedisManager::sUnion(const QVector<QString> &keys)
{
    return setOps_.sUnion(keys);
}

QVector<QString> RedisManager::sInter(const QVector<QString> &keys)
{
    return setOps_.sInter(keys);
}

QVector<QString> RedisManager::sDiff(const QVector<QString> &keys)
{
    return setOps_.sDiff(keys);
}

// Generic operations
bool RedisManager::del(const QString &key)
{
    return genericOps_.del(key);
}

bool RedisManager::exists(const QString &key)
{
    return genericOps_.exists(key);
}

QVector<QString> RedisManager::keys(const QString &pattern)
{
    return genericOps_.keys(pattern);
}

// Expiration operations
bool RedisManager::expire(const QString &key, int seconds)
{
    return expirationOps_.expire(key, seconds);
}

bool RedisManager::expireAt(const QString &key, qint64 timestamp)
{
    return expirationOps_.expireAt(key, timestamp);
}

int RedisManager::ttl(const QString &key)
{
    return expirationOps_.ttl(key);
}

bool RedisManager::persist(const QString &key)
{
    return expirationOps_.persist(key);
}

// Sorted Set operations
bool RedisManager::zAdd(const QString &key, double score, const QString &member)
{
    return sortedSetOps_.zAdd(key, score, member);
}

QVector<QString> RedisManager::zRange(const QString &key, int start, int stop)
{
    return sortedSetOps_.zRange(key, start, stop);
}

double RedisManager::zScore(const QString &key, const QString &member)
{
    return sortedSetOps_.zScore(key, member);
}

long long RedisManager::zRank(const QString &key, const QString &member)
{
    return sortedSetOps_.zRank(key, member);
}

long long RedisManager::zRevRank(const QString &key, const QString &member)
{
    return sortedSetOps_.zRevRank(key, member);
}

// Transaction operations
void RedisManager::multi()
{
    transactionOps_.multi();
}

bool RedisManager::exec()
{
    return transactionOps_.exec();
}

void RedisManager::discard()
{
    transactionOps_.discard();
}

bool RedisManager::watch(const QString &key)
{
    return transactionOps_.watch(key);
}
