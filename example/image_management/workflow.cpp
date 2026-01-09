#include "workflow.h"
#include <QTextStream>
#include <QDebug>
#include "redismanager.h"
#include "src/image_manager.h"
#include "demos/image_upload_demo.h"
#include "demos/image_retrieve_demo.h"
#include "demos/image_model_demo.h"
#include "demos/image_search_demo.h"
#include "demos/image_management_demo.h"

namespace Workflow {

// Constants
const char* REDIS_HOST = "127.0.0.1";
const int REDIS_PORT = 6379;

// Demo function type
using DemoFunction = std::function<void(ImageManager&)>;

// Demo information structure
struct DemoInfo {
    DemoFunction function;
    QString name;
    QString description;
};

// Demo registry mapping workflow type to demo info
static const QMap<WorkflowType, DemoInfo> DEMO_REGISTRY = {
    {WorkflowType::Upload, {ImageUploadDemo::run, "Upload", "图片上传 - 从文件或qimage上传图片"}},
    {WorkflowType::Retrieve, {ImageRetrieveDemo::run, "Retrieve", "图片检索 - 检索图片数据,将数据存为文件"}},
    {WorkflowType::Metadata, {ImageModelDemo::run, "Metadata", "图片元数据 - 管理图片元数据和标签"}},
    {WorkflowType::Search, {ImageSearchDemo::run, "Search", "图片搜索 - 通过标签搜索图片"}},
    {WorkflowType::Complete, {ImageManagementDemo::run, "Complete", "一个完整个workflow: upload, retrieve, update, search, delete"}}
};

// String to workflow type mapping
static const QMap<QString, WorkflowType> STRING_TO_WORKFLOW = {
    {"1", WorkflowType::Upload},
    {"2", WorkflowType::Retrieve},
    {"3", WorkflowType::Metadata},
    {"4", WorkflowType::Search},
    {"5", WorkflowType::Complete},
    {"6", WorkflowType::All},
    {"0", WorkflowType::None},
    {"upload", WorkflowType::Upload},
    {"retrieve", WorkflowType::Retrieve},
    {"metadata", WorkflowType::Metadata},
    {"search", WorkflowType::Search},
    {"complete", WorkflowType::Complete},
    {"all", WorkflowType::All},
    {"--help", WorkflowType::Help},
    {"-h", WorkflowType::Help}
};

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

void printHelp()
{
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
}

QString getWorkflowName(WorkflowType type)
{
    switch (type) {
        case WorkflowType::Upload: return "Upload";
        case WorkflowType::Retrieve: return "Retrieve";
        case WorkflowType::Metadata: return "Metadata";
        case WorkflowType::Search: return "Search";
        case WorkflowType::Complete: return "Complete";
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
    if (ok && num >= 0 && num <= 6) {
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
        ImageManager imageManager(redisManager);
        demoFunc(imageManager);
        redisManager.disconnect();
    } else {
        qWarning() << "无法连接到 Redis 服务器";
    }
}

void runAllDemos()
{
    RedisManager redisManager;
    if (redisManager.connectToServer(REDIS_HOST, REDIS_PORT)) {
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

        case WorkflowType::Upload:
        case WorkflowType::Retrieve:
        case WorkflowType::Metadata:
        case WorkflowType::Search:
        case WorkflowType::Complete:
            if (DEMO_REGISTRY.contains(type)) {
                runDemo(DEMO_REGISTRY[type].function);
                return 0;
            }
            qWarning() << "未找到工作流:" << getWorkflowName(type);
            return 1;

        default:
            qWarning() << "\n无效选择。请使用有效选项 (0-6) 重新运行";
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
