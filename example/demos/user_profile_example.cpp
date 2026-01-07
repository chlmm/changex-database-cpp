#include "user_profile_example.h"
#include "redismanager.h"
#include <QDebug>

void UserProfileExample::createUserProfile(RedisManager& redis, const QString& userId,
                                            const QString& name, const QString& email, int age)
{
    redis.hSet(userId, "name", name);
    redis.hSet(userId, "email", email);
    redis.hSet(userId, "age", QString::number(age));
}

void UserProfileExample::updateUserField(RedisManager& redis, const QString& userId,
                                           const QString& field, const QString& value)
{
    redis.hSet(userId, field, value);
}

QMap<QString, QString> UserProfileExample::getUserProfile(RedisManager& redis, const QString& userId)
{
    return redis.hGetAll(userId);
}

QList<QString> UserProfileExample::getAllUserIds(RedisManager& redis)
{
    QVector<QString> allKeys = redis.keys("demo:users:*");
    return allKeys.toList();
}

void UserProfileExample::deleteUserProfile(RedisManager& redis, const QString& userId)
{
    redis.del(userId);
}

int UserProfileExample::countUserProfiles(RedisManager& redis)
{
    QList<QString> users = getAllUserIds(redis);
    return users.size();
}

void UserProfileExample::run(RedisManager& redis)
{
    qDebug() << "\n=== User Profile Management (Real-World Example) ===";
    qDebug() << "This example demonstrates managing user profiles using Redis Hash.";

    // Create multiple users
    qDebug() << "\n1. Creating user profiles:";
    createUserProfile(redis, "demo:users:1001", "Alice Johnson", "alice@example.com", 28);
    qDebug() << "   Created user 1001: Alice Johnson (28)";

    createUserProfile(redis, "demo:users:1002", "Bob Smith", "bob@example.com", 32);
    qDebug() << "   Created user 1002: Bob Smith (32)";

    createUserProfile(redis, "demo:users:1003", "Charlie Brown", "charlie@example.com", 45);
    qDebug() << "   Created user 1003: Charlie Brown (45)";

    createUserProfile(redis, "demo:users:1004", "Diana Ross", "diana@example.com", 29);
    qDebug() << "   Created user 1004: Diana Ross (29)";

    // Count users
    qDebug() << "\n2. Total user profiles:";
    int count = countUserProfiles(redis);
    qDebug() << "   Total users: " << count;

    // Get a specific user
    qDebug() << "\n3. Retrieving user profile for user 1002:";
    QMap<QString, QString> user = getUserProfile(redis, "demo:users:1002");
    qDebug() << "   User 1002 Profile:";
    for (auto it = user.constBegin(); it != user.constEnd(); ++it) {
        qDebug() << "     " << it.key() << ": " << it.value();
    }

    // Update user information
    qDebug() << "\n4. Updating user 1001's age from 28 to 29:";
    updateUserField(redis, "demo:users:1001", "age", "29");
    QString newAge = redis.hGet("demo:users:1001", "age");
    qDebug() << "   New age: " << newAge;

    // Add a new field to existing user
    qDebug() << "\n5. Adding 'city' field to user 1001:";
    updateUserField(redis, "demo:users:1001", "city", "New York");
    QString city = redis.hGet("demo:users:1001", "city");
    qDebug() << "   City: " << city;

    // Get updated user profile
    qDebug() << "\n6. Updated user 1001 profile:";
    user = getUserProfile(redis, "demo:users:1001");
    qDebug() << "   User 1001 Profile:";
    for (auto it = user.constBegin(); it != user.constEnd(); ++it) {
        qDebug() << "     " << it.key() << ": " << it.value();
    }

    // Batch update - add city to all users
    qDebug() << "\n7. Adding city information to all users:";
    updateUserField(redis, "demo:users:1002", "city", "Los Angeles");
    updateUserField(redis, "demo:users:1003", "city", "Chicago");
    updateUserField(redis, "demo:users:1004", "city", "Miami");
    qDebug() << "   City information added to all users";

    // Check user count
    qDebug() << "\n8. Verifying user count:";
    count = countUserProfiles(redis);
    qDebug() << "   Total users: " << count;

    // List all users
    qDebug() << "\n9. Listing all user IDs:";
    QList<QString> userIds = getAllUserIds(redis);
    for (const QString& userId : userIds) {
        QString name = redis.hGet(userId, "name");
        qDebug() << "   " << userId << " - " << name;
    }

    // Batch retrieve - get all user names
    qDebug() << "\n10. Batch operation - retrieving all user names:";
    qDebug() << "    User List:";
    for (const QString& userId : userIds) {
        QString name = redis.hGet(userId, "name");
        QString age = redis.hGet(userId, "age");
        qDebug() << "     " << name << " (Age: " << age << ")";
    }

    // Delete a user
    qDebug() << "\n11. Deleting user 1003:";
    deleteUserProfile(redis, "demo:users:1003");
    qDebug() << "   User 1003 deleted";

    // Verify deletion
    qDebug() << "\n12. Verifying deletion:";
    bool exists = redis.exists("demo:users:1003");
    qDebug() << "   User 1003 exists: " << (exists ? "Yes" : "No");

    count = countUserProfiles(redis);
    qDebug() << "   Remaining users: " << count;

    // Cleanup
    qDebug() << "\n13. Cleaning up all demo users:";
    deleteUserProfile(redis, "demo:users:1001");
    deleteUserProfile(redis, "demo:users:1002");
    deleteUserProfile(redis, "demo:users:1004");
    qDebug() << "   Cleanup complete";

    qDebug() << "\n=== User Profile Management Example Complete ===";
    qDebug() << "\nKey Takeaways:";
    qDebug() << "- Redis Hash is perfect for storing object-like data (user profiles)";
    qDebug() << "- Each field can be updated independently without reading/writing the entire object";
    qDebug() << "- Fast access to specific fields (e.g., just get the user's email)";
    qDebug() << "- Efficient memory usage for structured data";
}
