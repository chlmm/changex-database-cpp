#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <QTextStream>
#include "redismanager.h"
#include "src/image_manager.h"
#include "demos/image_upload_demo.h"
#include "demos/image_retrieve_demo.h"
#include "demos/image_model_demo.h"
#include "demos/image_search_demo.h"
#include "demos/image_management_demo.h"

void printMenu()
{
    qDebug() << "\nQt Redis Plus Plus Extended - Image Management Demo";
    qDebug() << "==================================================";
    qDebug() << "1. 图片上传 - 从文件或qimage上传图片";
    qDebug() << "2. 图片检索 - 检索图片数据,将数据存为文件";
    qDebug() << "3. 图片元数据 - 管理图片元数据和标签";
    qDebug() << "4. 图片搜索 - 通过标签搜索图片";
    qDebug() << "5. 一个完整个workflow: upload, retrieve, update, search, delete";
    qDebug() << "6. 运行所有demos";
    qDebug() << "0. Exit";
    qDebug() << "\nSelect demo to run (0-6):";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Check for command line arguments
    if (argc > 1) {
        QString arg = QString(argv[1]).toLower();

        if (arg == "upload" || arg == "1") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ImageManager imageManager(redisManager);
                ImageUploadDemo::run(imageManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "retrieve" || arg == "2") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ImageManager imageManager(redisManager);
                ImageRetrieveDemo::run(imageManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "metadata" || arg == "3") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ImageManager imageManager(redisManager);
                ImageModelDemo::run(imageManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "search" || arg == "4") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ImageManager imageManager(redisManager);
                ImageSearchDemo::run(imageManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "complete" || arg == "5") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ImageManager imageManager(redisManager);
                ImageManagementDemo::run(imageManager);
                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "all" || arg == "6") {
            RedisManager redisManager;
            if (redisManager.connectToServer("127.0.0.1", 6379)) {
                ImageManager imageManager(redisManager);

                ImageUploadDemo::run(imageManager);
                qDebug() << "\n";

                ImageRetrieveDemo::run(imageManager);
                qDebug() << "\n";

                ImageModelDemo::run(imageManager);
                qDebug() << "\n";

                ImageSearchDemo::run(imageManager);
                qDebug() << "\n";

                ImageManagementDemo::run(imageManager);

                redisManager.disconnect();
            } else {
                qWarning() << "无法连接到 Redis 服务器";
            }
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        }

        if (arg == "--help" || arg == "-h") {
            qDebug() << "用法: ./image_management_example [选项]";
            qDebug() << "选项:";
            qDebug() << "  (无参数)  运行交互式菜单模式";
            qDebug() << "  upload, 1      图片上传";
            qDebug() << "  retrieve, 2    图片检索";
            qDebug() << "  metadata, 3    图片元数据";
            qDebug() << "  search, 4      图片搜索";
            qDebug() << "  complete, 5    一个完整个workflow";
            qDebug() << "  all, 6         运行所有demos";
            qDebug() << "  --help, -h     help";
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

    // Create image manager
    ImageManager imageManager(redisManager);

    switch (choice) {
        case 1:
            ImageUploadDemo::run(imageManager);
            break;
        case 2:
            ImageRetrieveDemo::run(imageManager);
            break;
        case 3:
            ImageModelDemo::run(imageManager);
            break;
        case 4:
            ImageSearchDemo::run(imageManager);
            break;
        case 5:
            ImageManagementDemo::run(imageManager);
            break;
        case 6:
            ImageUploadDemo::run(imageManager);
            qDebug() << "\n";
            ImageRetrieveDemo::run(imageManager);
            qDebug() << "\n";
            ImageModelDemo::run(imageManager);
            qDebug() << "\n";
            ImageSearchDemo::run(imageManager);
            qDebug() << "\n";
            ImageManagementDemo::run(imageManager);
            break;
        case 0:
            qDebug() << "\n退出中...";
            redisManager.disconnect();
            QTimer::singleShot(100, &app, &QCoreApplication::quit);
            return app.exec();
        default:
            qWarning() << "\n无效选择。请使用有效选项 (0-6) 重新运行";
            break;
    }

    // Disconnect
    redisManager.disconnect();

    qDebug() << "\n演示成功完成!";

    // Exit the application
    QTimer::singleShot(100, &app, &QCoreApplication::quit);
    return app.exec();
}
