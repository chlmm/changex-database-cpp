#include "set_demo.h"
#include "redismanager.h"
#include <QDebug>

void SetDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== Set Operations ===";
    qDebug() << "Demonstrating Redis Set operations:";

    // Create first set
    qDebug() << "\n[SADD] Creating set 'demo:set1':";
    redis.sAdd("demo:set1", "member1");
    qDebug() << "  Added: member1";
    redis.sAdd("demo:set1", "member2");
    qDebug() << "  Added: member2";
    redis.sAdd("demo:set1", "member3");
    qDebug() << "  Added: member3";

    // Try to add duplicate (should be ignored)
    qDebug() << "\n[SADD] Trying to add duplicate 'member1' (should be ignored):";
    redis.sAdd("demo:set1", "member1");
    qDebug() << "  Duplicate add - no effect";

    // Get set cardinality
    qDebug() << "\n[SCARD] Getting set1 size:";
    int size = redis.sCard("demo:set1");
    qDebug() << "  Set1 size: " << size;

    // Get all members
    qDebug() << "\n[SMEMBERS] Getting all members of set1:";
    QVector<QString> members = redis.sMembers("demo:set1");
    qDebug() << "  Members:" << members;

    // Check if member exists
    qDebug() << "\n[SISMEMBER] Checking membership:";
    bool isMember = redis.sIsMember("demo:set1", "member2");
    qDebug() << "  Is 'member2' in set1? " << (isMember ? "Yes" : "No");

    isMember = redis.sIsMember("demo:set1", "member5");
    qDebug() << "  Is 'member5' in set1? " << (isMember ? "Yes" : "No");

    // Create second set
    qDebug() << "\n[SADD] Creating set 'demo:set2':";
    redis.sAdd("demo:set2", "member2");
    qDebug() << "  Added: member2";
    redis.sAdd("demo:set2", "member3");
    qDebug() << "  Added: member3";
    redis.sAdd("demo:set2", "member4");
    qDebug() << "  Added: member4";

    size = redis.sCard("demo:set2");
    qDebug() << "  Set2 size: " << size;

    // Remove a member
    qDebug() << "\n[SREM] 从 set1 中移除 'member2':";
    redis.sRem("demo:set1", "member2");
    size = redis.sCard("demo:set1");
    qDebug() << "  移除后的 set1 大小: " << size;

    isMember = redis.sIsMember("demo:set1", "member2");
    qDebug() << "  Is 'member2' still in set1? " << (isMember ? "Yes" : "No");

    // Union of sets
    qDebug() << "\n[SUNION] Computing union of set1 and set2:";
    QVector<QString> keys;
    keys.append("demo:set1");
    keys.append("demo:set2");
    QVector<QString> unionSet = redis.sUnion(keys);
    qDebug() << "  Union: " << unionSet;

    // Intersection of sets
    qDebug() << "\n[SINTER] Computing intersection of set1 and set2:";
    QVector<QString> interSet = redis.sInter(keys);
    qDebug() << "  Intersection: " << interSet;

    // Difference (set1 - set2)
    qDebug() << "\n[SDIFF] Computing difference (set1 - set2):";
    QVector<QString> diffSet = redis.sDiff(keys);
    qDebug() << "  Difference (set1 - set2): " << diffSet;

    // Restore member2 for more demos
    qDebug() << "\n[SADD] Restoring 'member2' to set1 for demonstration:";
    redis.sAdd("demo:set1", "member2");

    // Get final state
    qDebug() << "\nFinal state:";
    members = redis.sMembers("demo:set1");
    qDebug() << "  Set1 members:" << members;

    QVector<QString> set2Members = redis.sMembers("demo:set2");
    qDebug() << "  Set2 members:" << set2Members;

    // Cleanup
    qDebug() << "\n[DEL] Cleaning up demo sets...";
    redis.del("demo:set1");
    redis.del("demo:set2");
    qDebug() << "  Cleanup complete";

    qDebug() << "\n=== Set Operations Demo Complete ===";
}
