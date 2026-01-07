#ifndef SET_DEMO_H
#define SET_DEMO_H

#include <QString>

class RedisManager;

class SetDemo {
public:
    static QString name() { return "Set Operations"; }
    static QString description() { return "Demonstrate Redis Set operations: sAdd, sMembers, sUnion, sInter, sDiff"; }

    static void run(RedisManager& redis);
};

#endif // SET_DEMO_H
