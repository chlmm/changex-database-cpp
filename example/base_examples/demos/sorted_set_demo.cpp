#include "sorted_set_demo.h"
#include <RedisModule/redismanager.h>
#include <QDebug>

void SortedSetDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== Sorted Set Operations ===";
    qDebug() << "Demonstrating Redis Sorted Set operations:";

    // Create a leaderboard
    qDebug() << "\n[ZADD] Creating leaderboard 'demo:leaderboard':";
    redis.zAdd("demo:leaderboard", 100.0, "player1");
    qDebug() << "  Added player1 with score 100.0";
    redis.zAdd("demo:leaderboard", 200.5, "player2");
    qDebug() << "  Added player2 with score 200.5";
    redis.zAdd("demo:leaderboard", 150.0, "player3");
    qDebug() << "  Added player3 with score 150.0";
    redis.zAdd("demo:leaderboard", 180.0, "player4");
    qDebug() << "  Added player4 with score 180.0";
    redis.zAdd("demo:leaderboard", 250.0, "player5");
    qDebug() << "  Added player5 with score 250.0";

    // Get range (ascending)
    qDebug() << "\n[ZRANGE] Getting all members (ascending order):";
    QVector<QString> leaders = redis.zRange("demo:leaderboard", 0, -1);
    qDebug() << "  Leaderboard (low to high):";
    for (int i = 0; i < leaders.size(); ++i) {
        double score = redis.zScore("demo:leaderboard", leaders[i]);
        qDebug() << "    [" << (i+1) << "] " << leaders[i] << " - Score: " << score;
    }

    // Get score of specific member
    qDebug() << "\n[ZSCORE] Getting score of specific players:";
    double score = redis.zScore("demo:leaderboard", "player3");
    qDebug() << "  player3 score: " << score;

    score = redis.zScore("demo:leaderboard", "player5");
    qDebug() << "  player5 score: " << score;

    // Get rank (ascending)
    qDebug() << "\n[ZRANK] Getting rank (ascending order):";
    int rank = redis.zRank("demo:leaderboard", "player3");
    qDebug() << "  player3 rank (0-indexed): " << rank;

    rank = redis.zRank("demo:leaderboard", "player5");
    qDebug() << "  player5 rank (0-indexed): " << rank;

    // Get reverse rank (descending)
    qDebug() << "\n[ZREVRANK] Getting reverse rank (descending order):";
    int revRank = redis.zRevRank("demo:leaderboard", "player3");
    qDebug() << "  player3 reverse rank (0-indexed): " << revRank;

    revRank = redis.zRevRank("demo:leaderboard", "player5");
    qDebug() << "  player5 reverse rank (0-indexed): " << revRank;

    // Get top 3 players (highest scores)
    qDebug() << "\n[ZREVRANGE] Getting top 3 players (highest scores):";
    QVector<QString> topPlayers = redis.zRange("demo:leaderboard", -3, -1);
    qDebug() << "  Top 3 players:";
    for (int i = 0; i < topPlayers.size(); ++i) {
        score = redis.zScore("demo:leaderboard", topPlayers[i]);
        qDebug() << "    [" << (i+1) << "] " << topPlayers[i] << " - Score: " << score;
    }

    // Update a player's score
    qDebug() << "\n[ZADD] 更新 player3 的分数从 150.0 到 175.0:";
    redis.zAdd("demo:leaderboard", 175.0, "player3");
    score = redis.zScore("demo:leaderboard", "player3");
    qDebug() << "  New score: " << score;

    // Get new rank
    rank = redis.zRank("demo:leaderboard", "player3");
    qDebug() << "  New rank (0-indexed): " << rank;

    // Get range after update
    qDebug() << "\n[ZRANGE] Updated leaderboard (ascending):";
    leaders = redis.zRange("demo:leaderboard", 0, -1);
    for (int i = 0; i < leaders.size(); ++i) {
        score = redis.zScore("demo:leaderboard", leaders[i]);
        qDebug() << "    [" << (i+1) << "] " << leaders[i] << " - Score: " << score;
    }

    // Get specific range (rank 2-4, 1-indexed: rank 3-5)
    qDebug() << "\n[ZRANGE] Getting players with rank 2-4 (0-indexed):";
    QVector<QString> rangePlayers = redis.zRange("demo:leaderboard", 2, 4);
    qDebug() << "  Players in rank range [2-4]:";
    for (int i = 0; i < rangePlayers.size(); ++i) {
        score = redis.zScore("demo:leaderboard", rangePlayers[i]);
        qDebug() << "    " << rangePlayers[i] << " - Score: " << score;
    }

    // Cleanup
    qDebug() << "\n[DEL] Cleaning up demo leaderboard...";
    redis.del("demo:leaderboard");
    qDebug() << "  Cleanup complete";

    qDebug() << "\n=== Sorted Set Operations Demo Complete ===";
}
