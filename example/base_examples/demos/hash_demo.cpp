#include "hash_demo.h"
#include "redismanager.h"
#include <QDebug>

void HashDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== Hash Operations ===";
    qDebug() << "Demonstrating Redis Hash operations:";

    // Create a hash with multiple fields
    qDebug() << "\n[HSET] Creating user profile hash 'demo:user:1001':";
    redis.hSet("demo:user:1001", "name", "Alice");
    redis.hSet("demo:user:1001", "age", "28");
    redis.hSet("demo:user:1001", "email", "alice@example.com");
    redis.hSet("demo:user:1001", "city", "New York");
    qDebug() << "  Created hash with fields: name, age, email, city";

    // Get hash length
    qDebug() << "\n[HLEN] Getting hash length:";
    int len = redis.hLen("demo:user:1001");
    qDebug() << "  Number of fields: " << len;

    // Get all fields and values
    qDebug() << "\n[HGETALL] Getting all fields and values:";
    QMap<QString, QString> user = redis.hGetAll("demo:user:1001");
    qDebug() << "  User profile:";
    for (auto it = user.constBegin(); it != user.constEnd(); ++it) {
        qDebug() << "    " << it.key() << ": " << it.value();
    }

    // Get specific field value
    qDebug() << "\n[HGET] Getting specific field 'email':";
    QString email = redis.hGet("demo:user:1001", "email");
    qDebug() << "  Result: " << email;

    // Get all keys
    qDebug() << "\n[HKEYS] Getting all field names:";
    QVector<QString> keys = redis.hKeys("demo:user:1001");
    qDebug() << "  Fields: " << keys;

    // Check if field exists
    qDebug() << "\n[HEXISTS] Checking field existence:";
    bool hasName = redis.hExists("demo:user:1001", "name");
    bool hasPhone = redis.hExists("demo:user:1001", "phone");
    qDebug() << "  Has 'name' field: " << (hasName ? "Yes" : "No");
    qDebug() << "  Has 'phone' field: " << (hasPhone ? "Yes" : "No");

    // Update a field
    qDebug() << "\n[HSET] 更新 'age' 字段从 28 到 29:";
    redis.hSet("demo:user:1001", "age", "29");
    QString age = redis.hGet("demo:user:1001", "age");
    qDebug() << "  New age: " << age;

    // Add new field
    qDebug() << "\n[HSET] 添加新字段 'phone':";
    redis.hSet("demo:user:1001", "phone", "555-1234");
    QString phone = redis.hGet("demo:user:1001", "phone");
    qDebug() << "  Phone: " << phone;

    // Check hash length after update
    qDebug() << "\n[HLEN] Hash length after adding field:";
    len = redis.hLen("demo:user:1001");
    qDebug() << "  Number of fields: " << len;

    // Delete a field
    qDebug() << "\n[HDEL] 删除 'phone' 字段:";
    redis.hDel("demo:user:1001", "phone");
    len = redis.hLen("demo:user:1001");
    qDebug() << "  删除后的字段数: " << len;

    // Create another hash for comparison
    qDebug() << "\n[HSET] Creating another user 'demo:user:1002':";
    redis.hSet("demo:user:1002", "name", "Bob");
    redis.hSet("demo:user:1002", "age", "32");
    redis.hSet("demo:user:1002", "email", "bob@example.com");
    qDebug() << "  Created user: Bob, age 32";

    // Cleanup
    qDebug() << "\n[DEL] Cleaning up demo hashes...";
    redis.del("demo:user:1001");
    redis.del("demo:user:1002");
    qDebug() << "  Cleanup complete";

    qDebug() << "\n=== Hash Operations Demo Complete ===";
}
