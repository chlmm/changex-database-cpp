#ifndef HASH_DEMO_H
#define HASH_DEMO_H

#include <QString>

class RedisManager;

class HashDemo {
public:
    static QString name() { return "Hash Operations"; }
    static QString description() { return "Demonstrate Redis Hash operations: hSet, hGet, hGetAll, hKeys, hDel"; }

    static void run(RedisManager& redis);
};

#endif // HASH_DEMO_H
