#include "list_demo.h"
#include <RedisModule/redismanager.h>
#include <QDebug>

void ListDemo::run(RedisManager& redis)
{
    qDebug() << "\n=== List Operations ===";
    qDebug() << "Demonstrating Redis List operations:";

    // Create a list with lPush (left push)
    qDebug() << "\n[LPUSH] Pushing items to the left of 'demo:mylist':";
    redis.lPush("demo:mylist", "item1");
    qDebug() << "  Pushed: item1";
    redis.lPush("demo:mylist", "item2");
    qDebug() << "  Pushed: item2";
    redis.lPush("demo:mylist", "item3");
    qDebug() << "  Pushed: item3";

    // Add items to the right
    qDebug() << "\n[RPUSH] Pushing items to the right of 'demo:mylist':";
    redis.rPush("demo:mylist", "item4");
    qDebug() << "  Pushed: item4";

    // Get list length
    qDebug() << "\n[LLEN] Getting list length:";
    int len = redis.lLen("demo:mylist");
    qDebug() << "  List length: " << len;

    // Get all elements
    qDebug() << "\n[LRANGE] Getting all elements (0 to -1):";
    QVector<QString> allItems = redis.lRange("demo:mylist", 0, -1);
    qDebug() << "  所有元素:";
    for (int i = 0; i < allItems.size(); ++i) {
        qDebug() << "    [" << i << "] " << allItems[i];
    }

    // Get range of elements
    qDebug() << "\n[LRANGE] Getting elements 1 to 3:";
    QVector<QString> rangeItems = redis.lRange("demo:mylist", 1, 3);
    qDebug() << "  元素[1-3]:";
    for (int i = 0; i < rangeItems.size(); ++i) {
        qDebug() << "    [" << (i+1) << "] " << rangeItems[i];
    }

    // Pop from left
    qDebug() << "\n[LPOP] Popping element from left:";
    QString leftPop = redis.lPop("demo:mylist");
    qDebug() << "  Popped: " << leftPop;
    len = redis.lLen("demo:mylist");
    qDebug() << "  List length after pop: " << len;

    // Pop from right
    qDebug() << "\n[RPOP] Popping element from right:";
    QString rightPop = redis.rPop("demo:mylist");
    qDebug() << "  Popped: " << rightPop;
    len = redis.lLen("demo:mylist");
    qDebug() << "  List length after pop: " << len;

    // Get element at specific index
    qDebug() << "\n[LINDEX] Getting element at index 0:";
    QString first = redis.lIndex("demo:mylist", 0);
    qDebug() << "  Element[0]: " << first;

    qDebug() << "\n[LINDEX] Getting element at index 1:";
    QString second = redis.lIndex("demo:mylist", 1);
    qDebug() << "  Element[1]: " << second;

    // Try to get non-existent element
    qDebug() << "\n[LINDEX] Trying to get element at index 10 (out of range):";
    QString outOfRange = redis.lIndex("demo:mylist", 10);
    qDebug() << "  Result: " << (outOfRange.isEmpty() ? "(empty/out of range)" : outOfRange);

    // Add more items to demonstrate queue behavior
    qDebug() << "\n[RPUSH] 添加更多项目以模拟队列:";
    redis.rPush("demo:mylist", "task1");
    redis.rPush("demo:mylist", "task2");
    redis.rPush("demo:mylist", "task3");
    qDebug() << "  添加了: task1, task2, task3";

    len = redis.lLen("demo:mylist");
    qDebug() << "\n[LLEN] Current list length: " << len;

    // Get current list state
    qDebug() << "\n[LRANGE] Current list state:";
    allItems = redis.lRange("demo:mylist", 0, -1);
    for (int i = 0; i < allItems.size(); ++i) {
        qDebug() << "    [" << i << "] " << allItems[i];
    }

    // Cleanup
    qDebug() << "\n[DEL] Cleaning up demo list...";
    redis.del("demo:mylist");
    qDebug() << "  Cleanup complete";

    qDebug() << "\n=== List Operations Demo Complete ===";
}
