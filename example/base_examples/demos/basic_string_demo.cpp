#include "basic_string_demo.h"
#include "redismanager.h"
#include <QDebug>

void BasicStringDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== String Operations ===";
    qDebug() << "Demonstrating basic Redis string operations:";

    // Set a string value
    qDebug() << "\n[SET] 设置键 \"demo:key1\" 为 \"Hello, Redis!\"";
    redis.set("demo:key1", "Hello, Redis!");

    // Get the string value
    qDebug() << "[GET] 获取 'demo:key1' 的值:";
    QString value = redis.get("demo:key1");
    qDebug() << "  结果: " << value;

    // Update the value
    qDebug() << "\n[SET] 更新 \"demo:key1\" 为 \"Hello, Redis Plus Plus!\"";
    redis.set("demo:key1", "Hello, Redis Plus Plus!");

    qDebug() << "[GET] 获取更新后的值:";
    value = redis.get("demo:key1");
    qDebug() << "  结果: " << value;

    // Check if key exists
    qDebug() << "\n[EXISTS] 检查 'demo:key1' 是否存在:";
    bool exists = redis.exists("demo:key1");
    qDebug() << "  存在: " << (exists ? "是" : "否");

    qDebug() << "\n[EXISTS] 检查 'demo:nonexistent' 是否存在:";
    exists = redis.exists("demo:nonexistent");
    qDebug() << "  存在: " << (exists ? "是" : "否");

    // Create multiple keys
    qDebug() << "\n[SET] 创建多个字符串键:";
    redis.set("demo:user:name", "Alice");
    redis.set("demo:user:age", "28");
    redis.set("demo:user:email", "alice@example.com");
    qDebug() << "  已创建: demo:user:name, demo:user:age, demo:user:email";

    // Delete a key
    qDebug() << "\n[DEL] 删除 'demo:nonexistent':";
    redis.del("demo:nonexistent");

    // Cleanup
    qDebug() << "\n[DEL] 清理演示键...";
    redis.del("demo:key1");
    redis.del("demo:user:name");
    redis.del("demo:user:age");
    redis.del("demo:user:email");
    qDebug() << "  清理完成";

    qDebug() << "\n=== String Operations Demo Complete ===";
}
