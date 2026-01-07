#ifndef SORTED_SET_DEMO_H
#define SORTED_SET_DEMO_H

#include <QString>

class RedisManager;

class SortedSetDemo {
public:
    static QString name() { return "Sorted Set Operations"; }
    static QString description() { return "Demonstrate Redis Sorted Set operations: zAdd, zRange, zScore, zRank"; }

    static void run(RedisManager& redis);
};

#endif // SORTED_SET_DEMO_H
