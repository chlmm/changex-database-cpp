#include "basic_string_demo.h"
#include "redismanager.h"
#include <QDebug>

void BasicStringDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== String Operations ===";
    qDebug() << "Demonstrating basic Redis string operations:";

    // Set a string value
    qDebug() << "\n[SET] Setting key 'demo:key1' to 'Hello, Redis!'";
    redis.set("demo:key1", "Hello, Redis!");

    // Get the string value
    qDebug() << "[GET] Retrieving value of 'demo:key1':";
    QString value = redis.get("demo:key1");
    qDebug() << "  Result: " << value;

    // Update the value
    qDebug() << "\n[SET] Updating 'demo:key1' to 'Hello, Redis Plus Plus!'";
    redis.set("demo:key1", "Hello, Redis Plus Plus!");

    qDebug() << "[GET] Retrieving updated value:";
    value = redis.get("demo:key1");
    qDebug() << "  Result: " << value;

    // Check if key exists
    qDebug() << "\n[EXISTS] Checking if 'demo:key1' exists:";
    bool exists = redis.exists("demo:key1");
    qDebug() << "  Exists: " << (exists ? "Yes" : "No");

    qDebug() << "\n[EXISTS] Checking if 'demo:nonexistent' exists:";
    exists = redis.exists("demo:nonexistent");
    qDebug() << "  Exists: " << (exists ? "Yes" : "No");

    // Create multiple keys
    qDebug() << "\n[SET] Creating multiple string keys:";
    redis.set("demo:user:name", "Alice");
    redis.set("demo:user:age", "28");
    redis.set("demo:user:email", "alice@example.com");
    qDebug() << "  Created: demo:user:name, demo:user:age, demo:user:email";

    // Delete a key
    qDebug() << "\n[DEL] Deleting 'demo:nonexistent':";
    redis.del("demo:nonexistent");

    // Cleanup
    qDebug() << "\n[DEL] Cleaning up demo keys...";
    redis.del("demo:key1");
    redis.del("demo:user:name");
    redis.del("demo:user:age");
    redis.del("demo:user:email");
    qDebug() << "  Cleanup complete";

    qDebug() << "\n=== String Operations Demo Complete ===";
}
