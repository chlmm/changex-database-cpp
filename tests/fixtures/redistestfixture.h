#ifndef REDISTESTFIXTURE_H
#define REDISTESTFIXTURE_H

#include <QObject>
#include <QString>
#include "../../RedisModule/redismanager.h"

class RedisTestFixture
{
public:
    RedisTestFixture();
    ~RedisTestFixture();

    bool connect();
    void cleanup();
    RedisManager* manager();

    static QString generateUniqueKey(const QString &prefix = "test");

private:
    RedisManager *manager_;
    QStringList keysToCleanup_;
    static int keyCounter_;
};

#endif // REDISTESTFIXTURE_H
