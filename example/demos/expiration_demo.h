#ifndef EXPIRATION_DEMO_H
#define EXPIRATION_DEMO_H

#include <QString>

class RedisManager;

class ExpirationDemo {
public:
    static QString name() { return "Expiration Operations"; }
    static QString description() { return "Demonstrate Redis key expiration operations: expire, ttl, persist"; }

    static void run(RedisManager& redis);
};

#endif // EXPIRATION_DEMO_H
