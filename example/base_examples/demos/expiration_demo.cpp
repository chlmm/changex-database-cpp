#include "expiration_demo.h"
#include <RedisModule/redismanager.h>
#include <QDebug>

void ExpirationDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== Expiration Operations ===";
    qDebug() << "Demonstrating Redis key expiration operations:";

    // Set a key without expiration
    qDebug() << "\n[SET] Creating key 'demo:temp1' without expiration:";
    redis.set("demo:temp1", "This key has no expiration");

    // Check TTL (Time To Live)
    qDebug() << "\n[TTL] Checking TTL of 'demo:temp1':";
    int ttl = redis.ttl("demo:temp1");
    qDebug() << "  TTL: " << ttl << " (-1 means no expiration)";

    // Set a key with 30 seconds expiration
    qDebug() << "\n[SETEX] Creating key 'demo:temp2' with 30 second expiration:";
    redis.set("demo:temp2", "This key expires in 30 seconds");
    redis.expire("demo:temp2", 30);
    qDebug() << "  Key set with 30 second expiration";

    // Check TTL
    qDebug() << "\n[TTL] Checking TTL of 'demo:temp2':";
    ttl = redis.ttl("demo:temp2");
    qDebug() << "  TTL: " << ttl << " seconds";

    // Set expiration on existing key
    qDebug() << "\n[EXPIRE] Setting 10 second expiration on 'demo:temp1':";
    bool success = redis.expire("demo:temp1", 10);
    qDebug() << "  Expiration set: " << (success ? "Success" : "Failed");

    qDebug() << "\n[TTL] Checking TTL of 'demo:temp1' after EXPIRE:";
    ttl = redis.ttl("demo:temp1");
    qDebug() << "  TTL: " << ttl << " seconds";

    // Set a key with 60 seconds expiration
    qDebug() << "\n[SET] Creating key 'demo:temp3':";
    redis.set("demo:temp3", "This key will get 15 second expiration");

    qDebug() << "\n[TTL] Checking initial TTL (should be -1):";
    ttl = redis.ttl("demo:temp3");
    qDebug() << "  TTL: " << ttl;

    qDebug() << "\n[EXPIRE] Setting 15 second expiration:";
    success = redis.expire("demo:temp3", 15);
    qDebug() << "  Expiration set: " << (success ? "Success" : "Failed");

    qDebug() << "\n[TTL] Checking TTL after EXPIRE:";
    ttl = redis.ttl("demo:temp3");
    qDebug() << "  TTL: " << ttl << " seconds";

    // Remove expiration
    qDebug() << "\n[PERSIST] Removing expiration from 'demo:temp1':";
    success = redis.persist("demo:temp1");
    qDebug() << "  Persist: " << (success ? "Success (expiration removed)" : "Failed (no expiration to remove)");

    qDebug() << "\n[TTL] Checking TTL of 'demo:temp1' after PERSIST:";
    ttl = redis.ttl("demo:temp1");
    qDebug() << "  TTL: " << ttl << " (-1 means no expiration)";

    // Try to persist a key without expiration
    qDebug() << "\n[PERSIST] Trying to persist 'demo:temp1' again (already has no expiration):";
    success = redis.persist("demo:temp1");
    qDebug() << "  Persist: " << (success ? "Success" : "Failed (no expiration to remove)");

    // Check TTL of all demo keys
    qDebug() << "\nChecking TTL of all demo keys:";
    ttl = redis.ttl("demo:temp1");
    qDebug() << "  demo:temp1 TTL: " << ttl;

    ttl = redis.ttl("demo:temp2");
    qDebug() << "  demo:temp2 TTL: " << ttl;

    ttl = redis.ttl("demo:temp3");
    qDebug() << "  demo:temp3 TTL: " << ttl;

    // Check existence
    qDebug() << "\n[EXISTS] Checking key existence:";
    bool exists = redis.exists("demo:temp1");
    qDebug() << "  demo:temp1 exists: " << (exists ? "Yes" : "No");

    exists = redis.exists("demo:temp2");
    qDebug() << "  demo:temp2 exists: " << (exists ? "Yes" : "No");

    exists = redis.exists("demo:temp3");
    qDebug() << "  demo:temp3 exists: " << (exists ? "Yes" : "No");

    // Cleanup
    qDebug() << "\n[DEL] Cleaning up demo keys...";
    redis.del("demo:temp1");
    redis.del("demo:temp2");
    redis.del("demo:temp3");
    qDebug() << "  Cleanup complete";

    qDebug() << "\n=== Expiration Operations Demo Complete ===";
}
