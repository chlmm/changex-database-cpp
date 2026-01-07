#ifndef USER_PROFILE_EXAMPLE_H
#define USER_PROFILE_EXAMPLE_H

#include <QString>
#include <QMap>

class RedisManager;

class UserProfileExample {
public:
    static QString name() { return "User Profile Management"; }
    static QString description() { return "Real-world example: Manage user profiles using Redis Hash"; }

    static void run(RedisManager& redis);

private:
    static void createUserProfile(RedisManager& redis, const QString& userId,
                                   const QString& name, const QString& email, int age);
    static void updateUserField(RedisManager& redis, const QString& userId,
                                const QString& field, const QString& value);
    static QMap<QString, QString> getUserProfile(RedisManager& redis, const QString& userId);
    static QList<QString> getAllUserIds(RedisManager& redis);
    static void deleteUserProfile(RedisManager& redis, const QString& userId);
    static int countUserProfiles(RedisManager& redis);
};

#endif // USER_PROFILE_EXAMPLE_H
