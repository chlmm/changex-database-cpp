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
    qDebug() << "1. String Operations - Basic Redis string operations";
    qDebug() << "2. Hash Operations - Demonstrate Redis Hash functionality";
    qDebug() << "3. List Operations - Demonstrate Redis List functionality";
    qDebug() << "4. Set Operations - Demonstrate Redis Set functionality";
    qDebug() << "5. Sorted Set Operations - Demonstrate Redis Sorted Set functionality";
    qDebug() << "6. Expiration Operations - Demonstrate key expiration";
    qDebug() << "7. User Profile Management (Real-World Example)";
    qDebug() << "8. Game Leaderboard (Real-World Example)";
    qDebug() << "9. Run All Demos";
    qDebug() << "0. Exit";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
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
                qWarning() << "Failed to connect to Redis server";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "--help" || arg == "-h") {
            qDebug() << "Usage: ./example [option]";
            qDebug() << "Options:";
            qDebug() << "  (no argument)  Run in interactive menu mode";
            qDebug() << "  string, 1      Run String Operations demo";
            qDebug() << "  hash, 2        Run Hash Operations demo";
            qDebug() << "  list, 3         Run List Operations demo";
            qDebug() << "  set, 4          Run Set Operations demo";
            qDebug()  << "  sorted_set, 5  Run Sorted Set Operations demo";
            qDebug() << "  expiration, 6   Run Expiration Operations demo";
            qDebug() << "  user_profile, 7 Run User Profile Management demo";
            qDebug() << "  leaderboard, 8  Run Game Leaderboard demo";
            qDebug() << "  all, 9         Run all demos";
            qDebug() << "  --help, -h     Show this help message";
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        qWarning() << "Unknown option:" << arg;
        qWarning() << "Use --help for usage information";
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
        qWarning() << "\nFailed to connect to Redis server.";
        qWarning() << "Make sure Redis is running on 127.0.0.1:6379";
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
            qDebug() << "\nExiting...";
            redisManager.disconnect();
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        default:
            qWarning() << "\nInvalid choice. Please run again with a valid option (0-9)";
            break;
    }

    // Disconnect
    redisManager.disconnect();

    qDebug() << "\nDemo completed successfully!";

    // Exit the application
    QTimer::singleShot(100, &app, &QCoreApplication::quit);
    return app.exec();
}
