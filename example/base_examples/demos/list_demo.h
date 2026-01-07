#ifndef LIST_DEMO_H
#define LIST_DEMO_H

#include <QString>

class RedisManager;

class ListDemo {
public:
    static QString name() { return "List Operations"; }
    static QString description() { return "Demonstrate Redis List operations: lPush, rPush, lPop, rPop, lRange, lIndex"; }

    static void run(RedisManager& redis);
};

#endif // LIST_DEMO_H
