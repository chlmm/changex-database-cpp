#include "leaderboard_example.h"
#include <RedisModule/redismanager.h>
#include <QDebug>

void LeaderboardExample::addPlayerScore(RedisManager& redis, const QString& leaderboardKey,
                                          const QString& playerId, double score)
{
    redis.zAdd(leaderboardKey, score, playerId);
}

QList<QPair<QString, double>> LeaderboardExample::getTopPlayers(RedisManager& redis,
                                                                    const QString& leaderboardKey, int count)
{
    QList<QPair<QString, double>> result;
    QVector<QString> topPlayers = redis.zRange(leaderboardKey, -count, -1);

    for (const QString& player : topPlayers) {
        double score = redis.zScore(leaderboardKey, player);
        result.append(qMakePair(player, score));
    }

    // Reverse to have highest scores first
    std::reverse(result.begin(), result.end());
    return result;
}

int LeaderboardExample::getPlayerRank(RedisManager& redis, const QString& leaderboardKey,
                                       const QString& playerId)
{
    int rank = redis.zRevRank(leaderboardKey, playerId);
    return rank + 1; // Convert to 1-indexed
}

QList<QPair<QString, double>> LeaderboardExample::getPlayersInRange(RedisManager& redis,
                                                                       const QString& leaderboardKey, int start, int end)
{
    QList<QPair<QString, double>> result;
    QVector<QString> players = redis.zRange(leaderboardKey, start, end);

    for (const QString& player : players) {
        double score = redis.zScore(leaderboardKey, player);
        result.append(qMakePair(player, score));
    }

    return result;
}

double LeaderboardExample::getPlayerScore(RedisManager& redis, const QString& leaderboardKey,
                                            const QString& playerId)
{
    return redis.zScore(leaderboardKey, playerId);
}

int LeaderboardExample::getPlayerCount(RedisManager& redis, const QString& leaderboardKey)
{
    // Use key existence check and approximate count
    return redis.exists(leaderboardKey) ? 0 : 0; // Note: This is a simplified check
}

void LeaderboardExample::run(RedisManager& redis)
{
    qDebug() << "\n=== Game Leaderboard (Real-World Example) ===";
    qDebug() << "This example demonstrates implementing a game leaderboard using Redis Sorted Set.";

    QString leaderboardKey = "demo:game:leaderboard";

    // Initialize leaderboard with player scores
    qDebug() << "\n1. Initializing leaderboard with player scores:";
    addPlayerScore(redis, leaderboardKey, "player1", 100.0);
    qDebug() << "   player1: 100.0";

    addPlayerScore(redis, leaderboardKey, "player2", 200.5);
    qDebug() << "   player2: 200.5";

    addPlayerScore(redis, leaderboardKey, "player3", 150.0);
    qDebug() << "   player3: 150.0";

    addPlayerScore(redis, leaderboardKey, "player4", 180.0);
    qDebug() << "   player4: 180.0";

    addPlayerScore(redis, leaderboardKey, "player5", 250.0);
    qDebug() << "   player5: 250.0";

    addPlayerScore(redis, leaderboardKey, "player6", 175.0);
    qDebug() << "   player6: 175.0";

    addPlayerScore(redis, leaderboardKey, "player7", 210.0);
    qDebug() << "   player7: 210.0";

    addPlayerScore(redis, leaderboardKey, "player8", 195.0);
    qDebug() << "   player8: 195.0";

    addPlayerScore(redis, leaderboardKey, "player9", 160.0);
    qDebug() << "   player9: 160.0";

    addPlayerScore(redis, leaderboardKey, "player10", 220.0);
    qDebug() << "   player10: 220.0";

    // Get top 5 players
    qDebug() << "\n2. Top 5 Players (Highest Scores):";
    QList<QPair<QString, double>> top5 = getTopPlayers(redis, leaderboardKey, 5);
    for (int i = 0; i < top5.size(); ++i) {
        qDebug() << "   [" << (i+1) << "] " << top5[i].first << " - Score: " << top5[i].second;
    }

    // Get player rank
    qDebug() << "\n3. Checking individual player ranks:";
    int rank = getPlayerRank(redis, leaderboardKey, "player3");
    qDebug() << "   player3 rank: " << rank;

    rank = getPlayerRank(redis, leaderboardKey, "player5");
    qDebug() << "   player5 rank: " << rank;

    rank = getPlayerRank(redis, leaderboardKey, "player1");
    qDebug() << "   player1 rank: " << rank;

    // Update player scores
    qDebug() << "\n4. Updating player scores:";
    qDebug() << "   player3 score updated from 150.0 to 230.0";
    addPlayerScore(redis, leaderboardKey, "player3", 230.0);

    qDebug() << "   player1 score updated from 100.0 to 180.0";
    addPlayerScore(redis, leaderboardKey, "player1", 180.0);

    // Check new ranks
    qDebug() << "\n5. New ranks after score updates:";
    rank = getPlayerRank(redis, leaderboardKey, "player3");
    qDebug() << "   player3 new rank: " << rank;

    rank = getPlayerRank(redis, leaderboardKey, "player1");
    qDebug() << "   player1 new rank: " << rank;

    // Get updated top 5
    qDebug() << "\n6. Updated Top 5 Players:";
    top5 = getTopPlayers(redis, leaderboardKey, 5);
    for (int i = 0; i < top5.size(); ++i) {
        qDebug() << "   [" << (i+1) << "] " << top5[i].first << " - Score: " << top5[i].second;
    }

    // Get players in rank range (6-10)
    qDebug() << "\n7. Players ranked 6th to 10th:";
    QList<QPair<QString, double>> rangePlayers = getPlayersInRange(redis, leaderboardKey, 5, 9);
    for (int i = 0; i < rangePlayers.size(); ++i) {
        int actualRank = i + 6;
        qDebug() << "   [" << actualRank << "] " << rangePlayers[i].first << " - Score: " << rangePlayers[i].second;
    }

    // Find players in score range
    qDebug() << "\n8. Finding players with score 170.0 to 210.0:";
    QVector<QString> allPlayers = redis.zRange(leaderboardKey, 0, -1);
    qDebug() << "   Players in score range [170.0, 210.0]:";
    for (const QString& player : allPlayers) {
        double score = getPlayerScore(redis, leaderboardKey, player);
        if (score >= 170.0 && score <= 210.0) {
            int r = getPlayerRank(redis, leaderboardKey, player);
            qDebug() << "     " << player << " - Score: " << score << " (Rank: " << r << ")";
        }
    }

    // Simulate real-time gameplay - multiple players scoring
    qDebug() << "\n9. Simulating real-time gameplay updates:";
    qDebug() << "   player6 earns 30 points!";
    addPlayerScore(redis, leaderboardKey, "player6", 205.0);

    qDebug() << "   player2 earns 15 points!";
    addPlayerScore(redis, leaderboardKey, "player2", 215.5);

    qDebug() << "   player9 earns 50 points!";
    addPlayerScore(redis, leaderboardKey, "player9", 210.0);

    // Final top 5
    qDebug() << "\n10. Final Top 5 Players after gameplay:";
    top5 = getTopPlayers(redis, leaderboardKey, 5);
    for (int i = 0; i < top5.size(); ++i) {
        qDebug() << "    [" << (i+1) << "] " << top5[i].first << " - Score: " << top5[i].second;
    }

    // Cleanup
    qDebug() << "\n11. Cleaning up demo leaderboard:";
    redis.del(leaderboardKey);
    qDebug() << "    Cleanup complete";

    qDebug() << "\n=== Leaderboard Example Complete ===";
    qDebug() << "\nKey Takeaways:";
    qDebug() << "- Redis Sorted Set maintains automatic sorting by score";
    qDebug() << "- Rankings are calculated in O(log N) time";
    qDebug() << "- Updates are instant and rankings adjust automatically";
    qDebug() << "- Efficient range queries (e.g., top 10, players 50-100)";
    qDebug() << "- Perfect for real-time scoring systems";
    qDebug() << "- No need to recalculate rankings after each update";
}
