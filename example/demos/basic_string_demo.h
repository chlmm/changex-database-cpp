#ifndef BASIC_STRING_DEMO_H
#define BASIC_STRING_DEMO_H

#include <QString>

class RedisManager;

class BasicStringDemo {
public:
    static QString name() { return "String Operations"; }
    static QString description() { return "Demonstrate basic Redis string operations: get, set, delete"; }

    static void run(RedisManager& redis);
};

#endif // BASIC_STRING_DEMO_H
