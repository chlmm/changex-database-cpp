#include "redistestfixture.h"
#include <QUuid>

int RedisTestFixture::keyCounter_ = 0;

RedisTestFixture::RedisTestFixture()
    : manager_(new RedisManager())
{
}

RedisTestFixture::~RedisTestFixture()
{
    cleanup();
    delete manager_;
}

bool RedisTestFixture::connect()
{
    return manager_->connectToServer("127.0.0.1", 6379);
}

void RedisTestFixture::cleanup()
{
    for (const QString &key : keysToCleanup_) {
        manager_->del(key);
    }
    keysToCleanup_.clear();
}

RedisManager* RedisTestFixture::manager()
{
    return manager_;
}

QString RedisTestFixture::generateUniqueKey(const QString &prefix)
{
    return QString("%1_%2_%3")
        .arg(prefix)
        .arg(++keyCounter_)
        .arg(QUuid::createUuid().toString(QUuid::WithoutBraces).left(8));
}
