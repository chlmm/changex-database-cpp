#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include <functional>

#include <RedisModule/redismanager.h>
#include "repositories/image_repository.h"
#include "scenarios/image_scenarios.h"

// ============ 配置 ============
const char* REDIS_HOST = "127.0.0.1";
const int REDIS_PORT = 6379;

// ============ 菜单结构 ============
struct MenuItem {
    QString key;
    QString title;
    QString description;
    std::function<void(ImageRepository&)> handler;
};

// ============ 菜单定义 ============
static const QList<MenuItem> MENU_ITEMS = {
    {"1", "上传图片", "从文件上传图片", ImageScenarios::uploadFromFile},
    {"2", "创建示例图片", "创建并上传示例图片 (红方块/渐变图/棋盘格等)", ImageScenarios::uploadSampleImage},
    {"3", "批量上传", "批量创建示例图片", ImageScenarios::uploadBatchSamples},
    {"", "", "", nullptr},  // 分隔线
    {"4", "列出所有图片", "显示所有图片的详细信息", ImageScenarios::listAllImages},
    {"5", "检索图片", "根据 ID 检索并显示图片详情", ImageScenarios::retrieveAndDisplay},
    {"6", "保存图片", "将图片保存到本地文件", ImageScenarios::saveImageToFile},
    {"7", "显示 QImage", "获取并显示 QImage 对象信息", ImageScenarios::displayAsQImage},
    {"8", "分页浏览", "分页浏览图片列表", ImageScenarios::browseWithPagination},
    {"9", "检查存在性", "检查图片是否存在", ImageScenarios::checkImageExistence},
    {"", "", "", nullptr},  // 分隔线
    {"10", "管理标签", "添加、删除或修改图片标签", ImageScenarios::manageTags},
    {"11", "更新元数据", "修改图片元数据信息", ImageScenarios::updateMetadata},
    {"", "", "", nullptr},  // 分隔线
    {"12", "标签搜索", "按单个标签搜索图片", ImageScenarios::searchByTag},
    {"13", "高级搜索", "多标签 AND/OR 搜索", ImageScenarios::advancedTagSearch},
    {"", "", "", nullptr},  // 分隔线
    {"14", "删除图片", "删除指定图片", ImageScenarios::deleteImage},
    {"15", "批量删除", "批量删除多张图片", ImageScenarios::deleteBatchImages},
    {"16", "清空所有", "删除所有图片数据", ImageScenarios::clearAllImages},
    {"", "", "", nullptr},  // 分隔线
    {"17", "统计信息", "显示图片库统计信息", ImageScenarios::showStatistics},
    {"18", "完整演示", "运行完整工作流演示", ImageScenarios::completeWorkflow},
};

// ============ 命令行映射 ============
static const QMap<QString, std::function<void(ImageRepository&)>> COMMAND_MAP = {
    {"upload", ImageScenarios::uploadFromFile},
    {"sample", ImageScenarios::uploadSampleImage},
    {"batch", ImageScenarios::uploadBatchSamples},
    {"list", ImageScenarios::listAllImages},
    {"get", ImageScenarios::retrieveAndDisplay},
    {"save", ImageScenarios::saveImageToFile},
    {"qimage", ImageScenarios::displayAsQImage},
    {"page", ImageScenarios::browseWithPagination},
    {"exists", ImageScenarios::checkImageExistence},
    {"tags", ImageScenarios::manageTags},
    {"update", ImageScenarios::updateMetadata},
    {"search", ImageScenarios::searchByTag},
    {"advsearch", ImageScenarios::advancedTagSearch},
    {"delete", ImageScenarios::deleteImage},
    {"deletebatch", ImageScenarios::deleteBatchImages},
    {"clear", ImageScenarios::clearAllImages},
    {"stats", ImageScenarios::showStatistics},
    {"demo", ImageScenarios::completeWorkflow},
};

// ============ 函数声明 ============
void printBanner();
void printMenu();
void printHelp();
void runInteractiveMode();
void runCommandMode(const QString& command);

// ============ 主函数 ============
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    printBanner();

    // 检查命令行参数
    QStringList args = app.arguments();
    if (args.size() > 1) {
        // 命令行模式
        QString command = args[1].toLower();
        if (command == "--help" || command == "-h") {
            printHelp();
            return 0;
        }
        runCommandMode(command);
    } else {
        // 交互式模式
        runInteractiveMode();
    }

    return 0;
}

// ============ 实现 ============

void printBanner()
{
    qDebug() << "";
    qDebug() << "╔══════════════════════════════════════════════════════════════╗";
    qDebug() << "║                                                              ║";
    qDebug() << "║     Redis Examples - 图片管理示例 (分层架构版)                ║";
    qDebug() << "║                                                              ║";
    qDebug() << "║     架构: Model → Repository → Scenario                      ║";
    qDebug() << "║                                                              ║";
    qDebug() << "╚══════════════════════════════════════════════════════════════╝";
    qDebug() << "";
}

void printMenu()
{
    qDebug() << "┌─────────────────────────────────────────────────────────────┐";
    qDebug() << "│                          功能菜单                            │";
    qDebug() << "├─────────────────────────────────────────────────────────────┤";
    
    for (const auto& item : MENU_ITEMS) {
        if (item.handler == nullptr) {
            // 分隔线
            qDebug() << "├─────────────────────────────────────────────────────────────┤";
        } else {
            QString line = QString("│ [%1] %-14s - %2").arg(item.key, -4).arg(item.title, -12).arg(item.description);
            // 填充空格到右边框
            int padding = 62 - line.length() + 3;  // +3 是为了补偿中文字符宽度
            qDebug() << qPrintable(line + QString(padding, ' ') + "│");
        }
    }
    
    qDebug() << "├─────────────────────────────────────────────────────────────┤";
    qDebug() << "│ [0]  退出程序                                                │";
    qDebug() << "└─────────────────────────────────────────────────────────────┘";
    qDebug() << "";
    std::cout << "请选择功能 (0-18): ";
}

void printHelp()
{
    qDebug() << "用法: ./redis_examples [命令]";
    qDebug() << "";
    qDebug() << "命令:";
    qDebug() << "  (无参数)     运行交互式菜单模式";
    qDebug() << "";
    qDebug() << "  upload       从文件上传图片";
    qDebug() << "  sample       创建示例图片 (红方块/渐变图/棋盘格等)";
    qDebug() << "  batch        批量上传示例图片";
    qDebug() << "  list         列出所有图片";
    qDebug() << "  get          检索图片详情";
    qDebug() << "  save         保存图片到文件";
    qDebug() << "  qimage       获取并显示 QImage 对象信息";
    qDebug() << "  page         分页浏览图片列表";
    qDebug() << "  exists       检查图片是否存在";
    qDebug() << "  tags         管理图片标签";
    qDebug() << "  update       更新图片元数据";
    qDebug() << "  search       按标签搜索";
    qDebug() << "  advsearch    高级标签搜索";
    qDebug() << "  delete       删除图片";
    qDebug() << "  deletebatch  批量删除图片";
    qDebug() << "  clear        清空所有图片";
    qDebug() << "  stats        显示统计信息";
    qDebug() << "  demo         运行完整演示";
    qDebug() << "";
    qDebug() << "  --help, -h   显示此帮助信息";
}

void runInteractiveMode()
{
    // 连接 Redis
    RedisManager redis;
    if (!redis.connectToServer(REDIS_HOST, REDIS_PORT)) {
        qDebug() << "错误: 无法连接到 Redis 服务器 (" << REDIS_HOST << ":" << REDIS_PORT << ")";
        qDebug() << "请确保 Redis 服务器已启动";
        return;
    }
    
    qDebug() << "已连接到 Redis 服务器 (" << REDIS_HOST << ":" << REDIS_PORT << ")";
    qDebug() << "";
    
    ImageRepository repo(redis);
    
    bool running = true;
    while (running) {
        printMenu();
        
        std::string input;
        std::getline(std::cin, input);
        QString choice = QString::fromStdString(input).trimmed();
        
        if (choice == "0") {
            running = false;
            qDebug() << "\n感谢使用，再见！";
        } else {
            // 查找对应的菜单项
            bool found = false;
            for (const auto& item : MENU_ITEMS) {
                if (item.key == choice && item.handler != nullptr) {
                    qDebug() << "";
                    item.handler(repo);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                qDebug() << "\n无效选择，请重新输入";
            }
            
            qDebug() << "";
        }
    }
    
    redis.disconnect();
}

void runCommandMode(const QString& command)
{
    // 连接 Redis
    RedisManager redis;
    if (!redis.connectToServer(REDIS_HOST, REDIS_PORT)) {
        qDebug() << "错误: 无法连接到 Redis 服务器";
        return;
    }
    
    ImageRepository repo(redis);
    
    // 查找并执行命令
    auto it = COMMAND_MAP.find(command);
    if (it != COMMAND_MAP.end()) {
        it.value()(repo);
    } else {
        qDebug() << "未知命令:" << command;
        qDebug() << "使用 --help 查看可用命令";
    }
    
    redis.disconnect();
}
