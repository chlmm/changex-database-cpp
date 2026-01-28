#include "workflow.h"
#include <QTextStream>
#include <QDebug>
#include <RedisModule/redismanager.h>
#include "demos/basic_string_demo.h"
#include "demos/hash_demo.h"
#include "demos/list_demo.h"
#include "demos/set_demo.h"
#include "demos/sorted_set_demo.h"
#include "demos/expiration_demo.h"
#include "demos/user_profile_example.h"
#include "demos/leaderboard_example.h"

namespace Workflow {

// Constants
const char* REDIS_HOST = "127.0.0.1";
const int REDIS_PORT = 6379;

// Demo function type
using DemoFunction = std::function<void(RedisManager&)>;

// Demo information structure
struct DemoInfo {
    DemoFunction function;
    QString name;
    QString description;
};

// Demo registry mapping workflow type to demo info
static const QMap<WorkflowType, DemoInfo> DEMO_REGISTRY = {
    {WorkflowType::String, {BasicStringDemo::run, "String", "String Operations - 基本 Redis 字符串操作"}},
    {WorkflowType::Hash, {HashDemo::run, "Hash", "Hash Operations - 演示 Redis Hash 功能"}},
    {WorkflowType::List, {ListDemo::run, "List", "List Operations - 演示 Redis List 功能"}},
    {WorkflowType::Set, {SetDemo::run, "Set", "Set Operations - 演示 Redis Set 功能"}},
    {WorkflowType::SortedSet, {SortedSetDemo::run, "SortedSet", "Sorted Set Operations - 演示 Redis Sorted Set 功能"}},
    {WorkflowType::Expiration, {ExpirationDemo::run, "Expiration", "Expiration Operations - 演示键过期"}},
    {WorkflowType::UserProfile, {UserProfileExample::run, "UserProfile", "User Profile Management (用户管理示例)"}},
    {WorkflowType::Leaderboard, {LeaderboardExample::run, "Leaderboard", "Game Leaderboard (排行榜示例)"}}
};

// String to workflow type mapping
static const QMap<QString, WorkflowType> STRING_TO_WORKFLOW = {
    {"1", WorkflowType::String},
    {"2", WorkflowType::Hash},
    {"3", WorkflowType::List},
    {"4", WorkflowType::Set},
    {"5", WorkflowType::SortedSet},
    {"6", WorkflowType::Expiration},
    {"7", WorkflowType::UserProfile},
    {"8", WorkflowType::Leaderboard},
    {"9", WorkflowType::All},
    {"0", WorkflowType::None},
    {"string", WorkflowType::String},
    {"hash", WorkflowType::Hash},
    {"list", WorkflowType::List},
    {"set", WorkflowType::Set},
    {"sorted_set", WorkflowType::SortedSet},
    {"sortedset", WorkflowType::SortedSet},
    {"expiration", WorkflowType::Expiration},
    {"user_profile", WorkflowType::UserProfile},
    {"userprofile", WorkflowType::UserProfile},
    {"leaderboard", WorkflowType::Leaderboard},
    {"all", WorkflowType::All},
    {"--help", WorkflowType::Help},
    {"-h", WorkflowType::Help}
};

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

void printHelp()
{
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
}

QString getWorkflowName(WorkflowType type)
{
    switch (type) {
        case WorkflowType::String: return "String";
        case WorkflowType::Hash: return "Hash";
        case WorkflowType::List: return "List";
        case WorkflowType::Set: return "Set";
        case WorkflowType::SortedSet: return "SortedSet";
        case WorkflowType::Expiration: return "Expiration";
        case WorkflowType::UserProfile: return "UserProfile";
        case WorkflowType::Leaderboard: return "Leaderboard";
        case WorkflowType::All: return "All";
        case WorkflowType::Help: return "Help";
        case WorkflowType::None: return "None";
        default: return "Unknown";
    }
}

WorkflowType parseWorkflowType(const QString& arg)
{
    QString lowerArg = arg.toLower().trimmed();

    // Check direct string mapping
    if (STRING_TO_WORKFLOW.contains(lowerArg)) {
        return STRING_TO_WORKFLOW[lowerArg];
    }

    // Try to convert to integer
    bool ok;
    int num = arg.toInt(&ok);
    if (ok && num >= 0 && num <= 9) {
        return STRING_TO_WORKFLOW[QString::number(num)];
    }

    qWarning() << "未知选项:" << arg;
    qWarning() << "使用 --help 查看用法信息";
    return WorkflowType::None;
}

void runDemo(const DemoFunction& demoFunc)
{
    RedisManager redisManager;
    if (redisManager.connectToServer(REDIS_HOST, REDIS_PORT)) {
        demoFunc(redisManager);
        redisManager.disconnect();
    } else {
        qWarning() << "无法连接到 Redis 服务器";
    }
}

void runAllDemos()
{
    RedisManager redisManager;
    if (redisManager.connectToServer(REDIS_HOST, REDIS_PORT)) {
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
}

int executeWorkflow(WorkflowType type)
{
    switch (type) {
        case WorkflowType::None:
            qDebug() << "\n退出中...";
            return 0;

        case WorkflowType::Help:
            printHelp();
            return 0;

        case WorkflowType::All:
            runAllDemos();
            return 0;

        case WorkflowType::String:
        case WorkflowType::Hash:
        case WorkflowType::List:
        case WorkflowType::Set:
        case WorkflowType::SortedSet:
        case WorkflowType::Expiration:
        case WorkflowType::UserProfile:
        case WorkflowType::Leaderboard:
            if (DEMO_REGISTRY.contains(type)) {
                runDemo(DEMO_REGISTRY[type].function);
                return 0;
            }
            qWarning() << "未找到工作流:" << getWorkflowName(type);
            return 1;

        default:
            qWarning() << "\n无效选择。请使用有效选项 (0-9) 重新运行";
            return 1;
    }
}

int executeWorkflow(const QString& arg)
{
    WorkflowType type = parseWorkflowType(arg);
    if (type == WorkflowType::None && arg.toLower().trimmed() != "0" &&
        arg.toLower().trimmed() != "--help" && arg.toLower().trimmed() != "-h") {
        return 1;
    }
    return executeWorkflow(type);
}

WorkflowType runInteractiveMenu()
{
    printMenu();

    QTextStream in(stdin);
    int choice = 0;
    in >> choice;

    return parseWorkflowType(QString::number(choice));
}

} // namespace Workflow
