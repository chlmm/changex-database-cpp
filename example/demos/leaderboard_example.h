#ifndef LEADERBOARD_EXAMPLE_H
#define LEADERBOARD_EXAMPLE_H

#include <QString>
#include <QList>
#include <QPair>

class RedisManager;

class LeaderboardExample {
public:
    static QString name() { return "Game Leaderboard"; }
    static QString description() { return "Real-world example: Implement game leaderboard using Redis Sorted Set"; }

    static void run(RedisManager& redis);

private:
    static void addPlayerScore(RedisManager& redis, const QString& leaderboardKey,
                                const QString& playerId, double score);
    static QList<QPair<QString, double>> getTopPlayers(RedisManager& redis,
                                                          const QString& leaderboardKey, int count);
    static int getPlayerRank(RedisManager& redis, const QString& leaderboardKey,
                             const QString& playerId);
    static QList<QPair<QString, double>> getPlayersInRange(RedisManager& redis,
                                                              const QString& leaderboardKey, int start, int end);
    static double getPlayerScore(RedisManager& redis, const QString& leaderboardKey,
                                  const QString& playerId);
    static int getPlayerCount(RedisManager& redis, const QString& leaderboardKey);
};

#endif // LEADERBOARD_EXAMPLE_H
