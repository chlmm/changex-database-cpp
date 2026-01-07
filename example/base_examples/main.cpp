#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QTextStream>
#include "redismanager.h"
#include "demos/basic_string_demo.h"
#include "demos/hash_demo.h"
#include "demos/list_demo.h"
#include "demos/set_demo.h"
#include "demos/sorted_set_demo.h"
#include "demos/expiration_demo.h"
#include "demos/user_profile_example.h"
#include "demos/leaderboard_example.h"

void printMenu()
{
    qDebug() << "\nQt Redis Plus Plus Extended Demo";
    qDebug() << "=================================";
    qDebug() << "Available Examples:";
    qDebug() << "1. String Operations - 基本 Redis 字符串操作";
    qDebug() << "2. Hash Operations - 演示 Redis Hash 功能";
    qDebug() << "3. List Operations - 演示 Redis List 功能";
    qDebug() << "4. Set Operations - 演示 Redis Set 功能";
    qDebug() << "5. Sorted Set Operations - 演示 Redis Sorted Set 功能";
    qDebug() << "6. Expiration Operations - 演示键过期";
    qDebug() << "7. User Profile Management (用户管理示例)";
    qDebug() << "8. Game Leaderboard (排行榜示例)";
    qDebug() << "9. 运行所有演示";
    qDebug() << "0. 退出";
    qDebug() << "\nSelect demo to run (0-9):";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Check for command line arguments
    if (argc > 1) {
        QString arg = QString(argv[1]).toLower();

        if (arg == "string" || arg == "1") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                BasicStringDemo::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "hash" || arg == "2") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                HashDemo::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "list" || arg == "3") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ListDemo::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "set" || arg == "4") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                SetDemo::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "sorted_set" || arg == "sortedset" || arg == "5") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                SortedSetDemo::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "expiration" || arg == "6") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ExpirationDemo::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "user_profile" || arg == "userprofile" || arg == "7") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                UserProfileExample::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "leaderboard" || arg == "8") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                LeaderboardExample::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "all" || arg == "9") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                BasicStringDemo::run(redisManager);
                qDebug() << "\n";
                HashDemo::run(redisManager);
                qDebug() << "\n";
                ListDemo::run(redisManager);
                qDebug() << "\n";
                SetDemo::run(redisManager);
                qDebug() << "\n";
                SortedSetDemo::run(redisManager);
                qDebug() << "\n";
                ExpirationDemo::run(redisManager);
                qDebug() << "\n";
                UserProfileExample::run(redisManager);
                qDebug() << "\n";
                LeaderboardExample::run(redisManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "--help" || arg == "-h") {
            qDebug() << "用法: ./example [选项]";
            qDebug() << "选项:";
            qDebug() << "  (无参数)  运行交互式菜单模式";
            qDebug() << "  string, 1      运行 String Operations 演示";
            qDebug() << "  hash, 2        运行 Hash Operations 演示";
            qDebug() << "  list, 3         运行 List Operations 演示";
            qDebug() << "  set, 4          运行 Set Operations 演示";
            qDebug()  << "  sorted_set, 5  运行 Sorted Set Operations 演示";
            qDebug() << "  expiration, 6   运行 Expiration Operations 演示";
            qDebug() << "  user_profile, 7 运行 User Profile Management 演示";
            qDebug() << "  leaderboard, 8  运行 Game Leaderboard 演示";
            qDebug() << "  all, 9         运行所有演示";
            qDebug() << "  --help, -h     显示此帮助信息";
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        qWarning() << "未知选项:" << arg;
        qWarning() << "使用 --help 查看用法信息";
        QTimer::singleShot(100, &app, &QCoreApplication::quit);
        return app.exec();
    }

    // Interactive menu mode
    printMenu();

    QTextStream in(stdin);
    int choice = 0;

    in >> choice;

    // Create Redis manager
    RedisManager redisManager;

    // Connect to Redis server
    if (!redisManager.connectToServer("127.0.0.1", 6379)) {
        qWarning() << "\n无法连接到 Redis 服务器。";
        qWarning() << "请确保 Redis 在 127.0.0.1:6379 上运行";
        QTimer::singleShot(100, &app, &QCoreApplication::quit);
        return app.exec();
    }

    switch (choice) {
        case 1:
            BasicStringDemo::run(redisManager);
            break;
        case 2:
            HashDemo::run(redisManager);
            break;
        case 3:
            ListDemo::run(redisManager);
            break;
        case 4:
            SetDemo::run(redisManager);
            break;
        case 5:
            SortedSetDemo::run(redisManager);
            break;
        case 6:
            ExpirationDemo::run(redisManager);
            break;
        case 7:
            UserProfileExample::run(redisManager);
            break;
        case 8:
            LeaderboardExample::run(redisManager);
            break;
        case 9:
            BasicStringDemo::run(redisManager);
            qDebug() << "\n";
            HashDemo::run(redisManager);
            qDebug() << "\n";
            ListDemo::run(redisManager);
            qDebug() << "\n";
            SetDemo::run(redisManager);
            qDebug() << "\n";
            SortedSetDemo::run(redisManager);
            qDebug() << "\n";
            ExpirationDemo::run(redisManager);
            qDebug() << "\n";
            UserProfileExample::run(redisManager);
            qDebug() << "\n";
            LeaderboardExample::run(redisManager);
            break;
        case 0:
            qDebug() << "\n退出中...";
            redisManager.disconnect();
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        default:
            qWarning() << "\n无效选择。请使用有效选项 (0-9) 重新运行";
            break;
    }

    // Disconnect
    redisManager.disconnect();

    qDebug() << "\n演示成功完成!";

    // Exit the application
    QTimer::singleShot(100, &app, &QCoreApplication::quit);
    return app.exec();
}
