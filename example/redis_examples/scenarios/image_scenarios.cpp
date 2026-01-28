#include "image_scenarios.h"
#include "../repositories/image_repository.h"
#include "../models/image_model.h"
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QDir>
#include <QTextStream>
#include <iostream>

// ============ 辅助函数 ============

static void printSeparator()
{
    qDebug() << "==================================================";
}

static void printHeader(const QString& title)
{
    printSeparator();
    qDebug() << title;
    printSeparator();
}

static QString readLine(const QString& prompt)
{
    std::cout << prompt.toStdString() << ": ";
    std::string input;
    std::getline(std::cin, input);
    return QString::fromStdString(input);
}

static int readInt(const QString& prompt, int defaultValue = 0)
{
    bool ok;
    int value = readLine(prompt + " [" + QString::number(defaultValue) + "]").toInt(&ok);
    return ok ? value : defaultValue;
}

static QStringList readTags(const QString& prompt)
{
    QString input = readLine(prompt + " (用逗号分隔多个标签，或直接回车跳过)");
    if (input.isEmpty()) {
        return QStringList();
    }
    return input.split(",", Qt::SkipEmptyParts);
}

static void waitForEnter()
{
    std::cout << "\n按回车键继续...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

// ============ 上传场景 ============

void ImageScenarios::uploadFromFile(ImageRepository& repo)
{
    printHeader("场景：从文件上传图片");

    QString filePath = readLine("请输入图片文件路径");
    if (filePath.isEmpty()) {
        qDebug() << "未提供文件路径，取消上传";
        return;
    }

    QStringList tags = readTags("请输入标签");

    qDebug() << "正在上传:" << filePath;
    QString id = repo.uploadFromFile(filePath, tags);

    if (!id.isEmpty()) {
        qDebug() << "上传成功！图片 ID:" << id;
    } else {
        qDebug() << "上传失败，请检查文件路径是否正确";
    }
    
    waitForEnter();
}

void ImageScenarios::uploadSampleImage(ImageRepository& repo)
{
    printHeader("场景：创建并上传示例图片");

    qDebug() << "请选择示例图片类型:";
    qDebug() << "  1. 红方块 (100x100, 红色)";
    qDebug() << "  2. 渐变图 (200x100, 彩色渐变)";
    qDebug() << "  3. 棋盘格 (150x150, 黑白格子)";
    qDebug() << "  4. 蓝矩形 (80x120, 蓝色)";
    qDebug() << "  5. 随机彩色块 (400x300)";
    
    QString choice = readLine("请选择 (1-5)");
    
    QImage image;
    QString filename;
    QStringList defaultTags;
    
    if (choice == "1") {
        // 红方块
        image = QImage(100, 100, QImage::Format_RGB32);
        image.fill(Qt::red);
        filename = "red_square.png";
        defaultTags << "simple" << "red";
    } else if (choice == "2") {
        // 渐变图
        image = QImage(200, 100, QImage::Format_RGB32);
        for (int y = 0; y < 100; ++y) {
            for (int x = 0; x < 200; ++x) {
                int r = (x * 255) / 200;
                int g = (y * 255) / 100;
                int b = 128;
                image.setPixelColor(x, y, QColor(r, g, b));
            }
        }
        filename = "gradient.png";
        defaultTags << "gradient" << "color";
    } else if (choice == "3") {
        // 棋盘格
        image = QImage(150, 150, QImage::Format_RGB32);
        const int cellSize = 25;
        for (int y = 0; y < 150; ++y) {
            for (int x = 0; x < 150; ++x) {
                bool black = ((x / cellSize + y / cellSize) % 2) == 0;
                image.setPixelColor(x, y, black ? Qt::black : Qt::white);
            }
        }
        filename = "checkerboard.png";
        defaultTags << "pattern" << "blackwhite";
    } else if (choice == "4") {
        // 蓝矩形
        image = QImage(80, 120, QImage::Format_RGB32);
        image.fill(Qt::blue);
        filename = "blue_rect.png";
        defaultTags << "simple" << "blue";
    } else {
        // 随机彩色块（原有功能）
        static int colorSeed = 0;
        image = QImage(400, 300, QImage::Format_RGB32);
        QColor bgColor((colorSeed * 50) % 255, (colorSeed * 80) % 255, (colorSeed * 110) % 255);
        colorSeed++;
        image.fill(bgColor);
        
        QPainter painter(&image);
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 20));
        painter.drawText(image.rect(), Qt::AlignCenter, QString("Sample Image %1").arg(colorSeed));
        painter.end();
        filename = QString("sample_%1.png").arg(colorSeed);
        defaultTags << "sample" << "test";
    }

    QStringList tags = readTags("请输入标签 (默认: " + defaultTags.join(", ") + ")");
    if (tags.isEmpty()) {
        tags = defaultTags;
    }

    qDebug() << "正在创建示例图片:" << filename;
    
    QString id = repo.uploadFromQImage(filename, image, "PNG", tags);

    if (!id.isEmpty()) {
        qDebug() << "示例图片上传成功！ID:" << id;
        ImageModel model = repo.findById(id);
        qDebug() << "图片信息:" << model.toString();
    } else {
        qDebug() << "上传失败";
    }
    
    waitForEnter();
}

void ImageScenarios::uploadBatchSamples(ImageRepository& repo)
{
    printHeader("场景：批量上传示例图片");

    qDebug() << "请选择批量上传模式:";
    qDebug() << "  1. 彩色方块序列 (颜色渐变的方块)";
    qDebug() << "  2. 混合示例图 (红方块、渐变图、棋盘格等组合)";
    
    QString mode = readLine("请选择 (1-2)");
    
    int count = readInt("请输入要创建的示例图片数量", 4);
    count = qBound(1, count, 10);

    QStringList baseTags = readTags("请输入基础标签");
    if (baseTags.isEmpty()) {
        baseTags << "batch" << "sample";
    }

    qDebug() << "开始批量创建" << count << "张示例图片...";

    QStringList createdIds;
    
    for (int i = 0; i < count; ++i) {
        QImage image;
        QString filename;
        QStringList tags = baseTags;
        
        if (mode == "2") {
            // 混合示例图模式 - 循环使用不同类型的图片
            int type = i % 4;
            switch (type) {
                case 0: {  // 红方块
                    image = QImage(100, 100, QImage::Format_RGB32);
                    image.fill(Qt::red);
                    filename = QString("batch%1_red.png").arg(i + 1);
                    tags << "red" << "square";
                    break;
                }
                case 1: {  // 渐变图
                    image = QImage(200, 100, QImage::Format_RGB32);
                    for (int y = 0; y < 100; ++y) {
                        for (int x = 0; x < 200; ++x) {
                            int r = ((x + i * 20) * 255) / 200 % 255;
                            int g = ((y + i * 30) * 255) / 100 % 255;
                            int b = 128;
                            image.setPixelColor(x, y, QColor(r, g, b));
                        }
                    }
                    filename = QString("batch%1_gradient.png").arg(i + 1);
                    tags << "gradient" << "color";
                    break;
                }
                case 2: {  // 棋盘格
                    image = QImage(150, 150, QImage::Format_RGB32);
                    const int cellSize = 25;
                    for (int y = 0; y < 150; ++y) {
                        for (int x = 0; x < 150; ++x) {
                            bool black = ((x / cellSize + y / cellSize + i) % 2) == 0;
                            image.setPixelColor(x, y, black ? Qt::black : Qt::white);
                        }
                    }
                    filename = QString("batch%1_checkerboard.png").arg(i + 1);
                    tags << "pattern" << "checkerboard";
                    break;
                }
                case 3: {  // 蓝矩形
                    image = QImage(80, 120, QImage::Format_RGB32);
                    image.fill(Qt::blue);
                    filename = QString("batch%1_blue.png").arg(i + 1);
                    tags << "blue" << "rect";
                    break;
                }
            }
        } else {
            // 彩色方块序列模式（默认）
            image = QImage(300, 200, QImage::Format_RGB32);
            QColor bgColor((i * 60) % 255, (i * 100) % 255, (i * 140) % 255);
            image.fill(bgColor);

            QPainter painter(&image);
            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 16));
            painter.drawText(image.rect(), Qt::AlignCenter, QString("Batch Image %1").arg(i + 1));
            painter.end();

            filename = QString("batch_%1.png").arg(i + 1);
            tags << QString("image%1").arg(i + 1);
        }
        
        QString id = repo.uploadFromQImage(filename, image, "PNG", tags);
        
        if (!id.isEmpty()) {
            createdIds << id;
            qDebug() << "  [" << (i + 1) << "/" << count << "] 上传成功:" << id.left(8) << "...";
        }
    }

    qDebug() << "批量上传完成，共创建" << createdIds.size() << "张图片";
    waitForEnter();
}

// ============ 检索场景 ============

void ImageScenarios::listAllImages(ImageRepository& repo)
{
    printHeader("场景：列出所有图片");

    QList<ImageModel> images = repo.findAll();
    
    if (images.isEmpty()) {
        qDebug() << "当前没有图片，请先上传一些图片";
        waitForEnter();
        return;
    }

    qDebug() << "共找到" << images.size() << "张图片:";
    qDebug() << "";
    
    for (int i = 0; i < images.size(); ++i) {
        const ImageModel& model = images[i];
        qDebug() << "[" << (i + 1) << "]";
        qDebug() << "    ID:" << model.getId();
        qDebug() << "    文件名:" << model.getFilename();
        qDebug() << "    类型:" << model.getMimeType();
        qDebug() << "    大小:" << model.getSizeFormatted();
        qDebug() << "    尺寸:" << model.getDimensionsString();
        qDebug() << "    标签:" << model.getTags().join(", ");
        qDebug() << "    上传时间:" << model.getUploadTime().toString("yyyy-MM-dd hh:mm:ss");
        qDebug() << "";
    }
    
    waitForEnter();
}

void ImageScenarios::retrieveAndDisplay(ImageRepository& repo)
{
    printHeader("场景：检索并显示图片详情");

    QString id = readLine("请输入图片 ID");
    if (id.isEmpty()) {
        qDebug() << "未提供 ID";
        return;
    }

    ImageModel model = repo.findById(id);
    
    if (!model.isValid()) {
        qDebug() << "未找到该图片:" << id;
        waitForEnter();
        return;
    }

    qDebug() << "图片详情:";
    qDebug() << "  ID:" << model.getId();
    qDebug() << "  文件名:" << model.getFilename();
    qDebug() << "  MIME 类型:" << model.getMimeType();
    qDebug() << "  文件大小:" << model.getSizeFormatted() << "(" << model.getSize() << " bytes)";
    qDebug() << "  图片尺寸:" << model.getDimensionsString();
    qDebug() << "  标签:" << (model.getTags().isEmpty() ? "无" : model.getTags().join(", "));
    qDebug() << "  上传时间:" << model.getUploadTime().toString("yyyy-MM-dd hh:mm:ss");

    // 尝试加载图片
    QImage image = repo.getImageAsQImage(id);
    if (!image.isNull()) {
        qDebug() << "  图片状态: 可以正常加载";
    } else {
        qDebug() << "  图片状态: 无法加载 (可能不是标准图片格式)";
    }
    
    waitForEnter();
}

void ImageScenarios::saveImageToFile(ImageRepository& repo)
{
    printHeader("场景：保存图片到本地文件");

    QString id = readLine("请输入图片 ID");
    if (id.isEmpty()) {
        qDebug() << "未提供 ID";
        return;
    }

    ImageModel model = repo.findById(id);
    if (!model.isValid()) {
        qDebug() << "未找到该图片";
        waitForEnter();
        return;
    }

    QString defaultPath = QDir::current().filePath(model.getFilename());
    QString filePath = readLine("请输入保存路径 [" + defaultPath + "]");
    if (filePath.isEmpty()) {
        filePath = defaultPath;
    }

    if (repo.saveToFile(id, filePath)) {
        qDebug() << "图片已保存到:" << filePath;
    } else {
        qDebug() << "保存失败";
    }
    
    waitForEnter();
}

// ============ 元数据管理场景 ============

void ImageScenarios::manageTags(ImageRepository& repo)
{
    printHeader("场景：管理图片标签");

    QString id = readLine("请输入图片 ID");
    if (id.isEmpty()) {
        qDebug() << "未提供 ID";
        return;
    }

    ImageModel model = repo.findById(id);
    if (!model.isValid()) {
        qDebug() << "未找到该图片";
        waitForEnter();
        return;
    }

    qDebug() << "当前标签:" << (model.getTags().isEmpty() ? "无" : model.getTags().join(", "));
    qDebug() << "";
    qDebug() << "操作选项:";
    qDebug() << "  1. 添加标签";
    qDebug() << "  2. 删除标签";
    qDebug() << "  3. 设置所有标签(覆盖)";
    
    QString choice = readLine("请选择操作 (1-3)");
    
    if (choice == "1") {
        QString tag = readLine("请输入要添加的标签");
        if (!tag.isEmpty()) {
            if (repo.addTag(id, tag)) {
                qDebug() << "标签添加成功";
            } else {
                qDebug() << "标签添加失败";
            }
        }
    } else if (choice == "2") {
        QString tag = readLine("请输入要删除的标签");
        if (!tag.isEmpty()) {
            if (repo.removeTag(id, tag)) {
                qDebug() << "标签删除成功";
            } else {
                qDebug() << "标签删除失败 (可能不存在)";
            }
        }
    } else if (choice == "3") {
        QStringList tags = readTags("请输入新标签列表(将覆盖现有标签)");
        if (repo.updateTags(id, tags)) {
            qDebug() << "标签更新成功";
        } else {
            qDebug() << "标签更新失败";
        }
    }

    // 显示更新后的标签
    model = repo.findById(id);
    qDebug() << "更新后标签:" << (model.getTags().isEmpty() ? "无" : model.getTags().join(", "));
    
    waitForEnter();
}

void ImageScenarios::updateMetadata(ImageRepository& repo)
{
    printHeader("场景：更新图片元数据");

    QString id = readLine("请输入图片 ID");
    if (id.isEmpty()) {
        qDebug() << "未提供 ID";
        return;
    }

    ImageModel model = repo.findById(id);
    if (!model.isValid()) {
        qDebug() << "未找到该图片";
        waitForEnter();
        return;
    }

    qDebug() << "当前文件名:" << model.getFilename();
    QString newFilename = readLine("新文件名 (直接回车保持不变)");
    if (!newFilename.isEmpty()) {
        model.setFilename(newFilename);
    }

    if (repo.update(model)) {
        qDebug() << "元数据更新成功";
    } else {
        qDebug() << "元数据更新失败";
    }
    
    waitForEnter();
}

// ============ 搜索场景 ============

void ImageScenarios::searchByTag(ImageRepository& repo)
{
    printHeader("场景：按标签搜索图片");

    QString tag = readLine("请输入要搜索的标签");
    if (tag.isEmpty()) {
        qDebug() << "未提供标签";
        return;
    }

    QList<ImageModel> results = repo.searchByTag(tag);
    
    qDebug() << "找到" << results.size() << "张带有标签 '" << tag << "' 的图片:";
    
    for (int i = 0; i < results.size(); ++i) {
        const ImageModel& model = results[i];
        qDebug() << "[" << (i + 1) << "]" << model.getId() 
                 << "-" << model.getFilename() 
                 << "[" << model.getTags().join(", ") << "]";
    }
    
    waitForEnter();
}

void ImageScenarios::advancedTagSearch(ImageRepository& repo)
{
    printHeader("场景：高级标签搜索");

    QStringList tags = readTags("请输入要搜索的标签");
    if (tags.isEmpty()) {
        qDebug() << "未提供标签";
        return;
    }

    if (tags.size() == 1) {
        // 单标签搜索
        QList<ImageModel> results = repo.searchByTag(tags.first());
        qDebug() << "找到" << results.size() << "张图片带有标签 '" << tags.first() << "'";
        for (const auto& model : results) {
            qDebug() << "  -" << model.getId() << model.getFilename();
        }
    } else {
        // 多标签搜索
        QString mode = readLine("搜索模式: 1=AND(同时包含所有标签) 2=OR(包含任一标签) [1]");
        bool matchAll = (mode != "2");
        
        QList<ImageModel> results = repo.searchByTags(tags, matchAll);
        QString modeStr = matchAll ? "AND" : "OR";
        
        qDebug() << "搜索模式:" << modeStr;
        qDebug() << "标签:" << tags.join(matchAll ? " + " : " | ");
        qDebug() << "找到" << results.size() << "张图片:";
        
        for (const auto& model : results) {
            qDebug() << "  -" << model.getId() << model.getFilename() 
                     << "[" << model.getTags().join(", ") << "]";
        }
    }
    
    waitForEnter();
}

// ============ 删除场景 ============

void ImageScenarios::deleteImage(ImageRepository& repo)
{
    printHeader("场景：删除图片");

    QString id = readLine("请输入要删除的图片 ID");
    if (id.isEmpty()) {
        qDebug() << "未提供 ID";
        return;
    }

    ImageModel model = repo.findById(id);
    if (!model.isValid()) {
        qDebug() << "未找到该图片";
        waitForEnter();
        return;
    }

    qDebug() << "将要删除的图片:";
    qDebug() << "  ID:" << model.getId();
    qDebug() << "  文件名:" << model.getFilename();
    qDebug() << "  大小:" << model.getSizeFormatted();

    QString confirm = readLine("确认删除? (输入 yes 确认)");
    if (confirm.toLower() != "yes") {
        qDebug() << "取消删除";
        waitForEnter();
        return;
    }

    if (repo.remove(id)) {
        qDebug() << "图片删除成功";
    } else {
        qDebug() << "图片删除失败";
    }
    
    waitForEnter();
}

void ImageScenarios::clearAllImages(ImageRepository& repo)
{
    printHeader("场景：清空所有图片");

    int count = repo.count();
    if (count == 0) {
        qDebug() << "当前没有图片";
        waitForEnter();
        return;
    }

    qDebug() << "警告：这将删除所有" << count << "张图片！";
    QString confirm = readLine("确认清空所有数据? (输入 DELETE 确认)");
    
    if (confirm != "DELETE") {
        qDebug() << "操作已取消";
        waitForEnter();
        return;
    }

    if (repo.clearAll()) {
        qDebug() << "所有图片已清空";
    } else {
        qDebug() << "清空操作完成 (可能有部分失败)";
    }
    
    waitForEnter();
}

// ============ 统计场景 ============

void ImageScenarios::showStatistics(ImageRepository& repo)
{
    printHeader("场景：统计信息");

    qDebug() << repo.getStatistics();
    
    int count = repo.count();
    if (count > 0) {
        qDebug() << "";
        qDebug() << "详细信息:";
        QList<ImageModel> images = repo.findAll();
        
        // 收集所有标签
        QSet<QString> allTags;
        for (const auto& model : images) {
            for (const QString& tag : model.getTags()) {
                allTags.insert(tag);
            }
        }
        qDebug() << "  不同标签数:" << allTags.size();
        if (!allTags.isEmpty()) {
            qDebug() << "  所有标签:" << QStringList(allTags.begin(), allTags.end()).join(", ");
        }
    }
    
    waitForEnter();
}

// ============ 完整工作流场景 ============

void ImageScenarios::completeWorkflow(ImageRepository& repo)
{
    printHeader("场景：完整工作流演示");
    qDebug() << "演示流程: 上传 → 检索 → 修改标签 → 搜索 → 删除";
    qDebug() << "";

    // 1. 上传
    qDebug() << "【步骤 1】上传示例图片...";
    QImage image(400, 300, QImage::Format_RGB32);
    image.fill(QColor(100, 150, 200));
    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 18));
    painter.drawText(image.rect(), Qt::AlignCenter, "Workflow Demo");
    painter.end();

    QStringList tags;
    tags << "workflow" << "demo" << "test";
    QString id = repo.uploadFromQImage("workflow_demo.png", image, "PNG", tags);
    
    if (id.isEmpty()) {
        qDebug() << "上传失败，终止演示";
        waitForEnter();
        return;
    }
    qDebug() << "上传成功，ID:" << id;
    qDebug() << "";

    // 2. 检索
    qDebug() << "【步骤 2】检索图片详情...";
    ImageModel model = repo.findById(id);
    qDebug() << "图片信息:" << model.toString();
    qDebug() << "";

    // 3. 修改标签
    qDebug() << "【步骤 3】添加新标签 'important'...";
    repo.addTag(id, "important");
    model = repo.findById(id);
    qDebug() << "更新后标签:" << model.getTags().join(", ");
    qDebug() << "";

    // 4. 搜索
    qDebug() << "【步骤 4】搜索带有 'important' 标签的图片...";
    QList<ImageModel> results = repo.searchByTag("important");
    qDebug() << "找到" << results.size() << "张图片:";
    for (const auto& m : results) {
        qDebug() << "  -" << m.getId() << m.getFilename();
    }
    qDebug() << "";

    // 5. 删除
    qDebug() << "【步骤 5】删除该图片...";
    if (repo.remove(id)) {
        qDebug() << "删除成功";
    }
    qDebug() << "";

    // 验证删除
    qDebug() << "【验证】再次搜索 'important'...";
    results = repo.searchByTag("important");
    qDebug() << "找到" << results.size() << "张图片 (应该为 0)";
    
    qDebug() << "";
    qDebug() << "完整工作流演示结束！";
    waitForEnter();
}

// ============ 新增的高级场景 ============

void ImageScenarios::displayAsQImage(ImageRepository& repo)
{
    printHeader("场景：获取并显示 QImage 对象");

    QStringList allIds;
    QList<ImageModel> images = repo.findAll();
    for (const auto& img : images) {
        allIds << img.getId();
    }
    
    if (allIds.isEmpty()) {
        qDebug() << "当前没有图片，请先上传一些图片";
        waitForEnter();
        return;
    }

    QString id = readLine("请输入图片 ID (直接回车使用第一张)");
    if (id.isEmpty()) {
        id = allIds.first();
    }

    ImageModel model = repo.findById(id);
    if (!model.isValid()) {
        qDebug() << "未找到该图片";
        waitForEnter();
        return;
    }

    qDebug() << "正在获取 QImage...";
    QImage image = repo.getImageAsQImage(id);

    if (!image.isNull()) {
        qDebug() << "成功获取 QImage!";
        qDebug() << "  图片尺寸:" << image.width() << "x" << image.height();
        qDebug() << "  格式:" << image.format();
        qDebug() << "  深度:" << image.depth() << "bits";
        qDebug() << "  每行字节数:" << image.bytesPerLine();
        qDebug() << "  总字节数:" << image.sizeInBytes();
        
        // 获取图片数据对比
        QByteArray rawData = repo.getImageData(id);
        qDebug() << "  原始数据大小:" << rawData.size() << "bytes";
    } else {
        qDebug() << "无法加载为 QImage (可能不是标准图片格式)";
    }
    
    waitForEnter();
}

void ImageScenarios::browseWithPagination(ImageRepository& repo)
{
    printHeader("场景：分页浏览图片");

    int totalCount = repo.count();
    if (totalCount == 0) {
        qDebug() << "当前没有图片";
        waitForEnter();
        return;
    }

    int pageSize = readInt("请输入每页显示数量", 3);
    pageSize = qMax(1, pageSize);
    
    int totalPages = (totalCount + pageSize - 1) / pageSize;
    int currentPage = 0;
    bool browsing = true;

    while (browsing) {
        int offset = currentPage * pageSize;
        QStringList pageIds = repo.findIdsPaginated(offset, pageSize);

        printHeader(QString("第 %1/%2 页 (共 %3 张图片)").arg(currentPage + 1).arg(totalPages).arg(totalCount));

        for (int i = 0; i < pageIds.size(); ++i) {
            ImageModel model = repo.findById(pageIds[i]);
            qDebug() << QString("[%1] %2 - %3 (%4)")
                        .arg(offset + i + 1)
                        .arg(model.getId().left(8))
                        .arg(model.getFilename())
                        .arg(model.getSizeFormatted());
        }

        qDebug() << "";
        qDebug() << "操作: [n]下一页 [p]上一页 [j]跳转 [q]退出";
        QString choice = readLine("请选择").toLower();

        if (choice == "n" || choice == "next") {
            if (currentPage < totalPages - 1) {
                currentPage++;
            } else {
                qDebug() << "已经是最后一页";
            }
        } else if (choice == "p" || choice == "prev") {
            if (currentPage > 0) {
                currentPage--;
            } else {
                qDebug() << "已经是第一页";
            }
        } else if (choice == "j" || choice == "jump") {
            int page = readInt(QString("请输入页码 (1-%1)").arg(totalPages), currentPage + 1);
            if (page >= 1 && page <= totalPages) {
                currentPage = page - 1;
            }
        } else if (choice == "q" || choice == "quit") {
            browsing = false;
        }
    }
}

void ImageScenarios::checkImageExistence(ImageRepository& repo)
{
    printHeader("场景：检查图片存在性");

    // 检查存在的图片
    QList<ImageModel> images = repo.findAll();
    if (!images.isEmpty()) {
        qDebug() << "检查真实存在的图片:";
        for (int i = 0; i < qMin(3, images.size()); ++i) {
            QString id = images[i].getId();
            bool exists = repo.exists(id);
            qDebug() << QString("  ID: %1... 存在: %2")
                        .arg(id.left(12))
                        .arg(exists ? "✓ Yes" : "✗ No");
        }
    }

    qDebug() << "";
    
    // 检查不存在的图片
    qDebug() << "检查不存在的图片:";
    QStringList fakeIds;
    fakeIds << "non-existent-id-12345"
            << "fake-image-id-99999"
            << "invalid-id-test";
    
    for (const QString& fakeId : fakeIds) {
        bool exists = repo.exists(fakeId);
        qDebug() << QString("  ID: %1... 存在: %2")
                    .arg(fakeId)
                    .arg(exists ? "✓ Yes (unexpected!)" : "✗ No");
    }

    // 允许用户输入自定义ID检查
    qDebug() << "";
    QString customId = readLine("输入图片 ID 检查存在性 (直接回车跳过)");
    if (!customId.isEmpty()) {
        bool exists = repo.exists(customId);
        qDebug() << QString("ID: %1... 存在: %2")
                    .arg(customId.left(20))
                    .arg(exists ? "✓ Yes" : "✗ No");
        
        if (exists) {
            ImageModel model = repo.findById(customId);
            qDebug() << "图片信息:" << model.toString();
        }
    }
    
    waitForEnter();
}

void ImageScenarios::deleteBatchImages(ImageRepository& repo)
{
    printHeader("场景：批量删除图片");

    QList<ImageModel> images = repo.findAll();
    if (images.isEmpty()) {
        qDebug() << "当前没有图片";
        waitForEnter();
        return;
    }

    qDebug() << "当前共有" << images.size() << "张图片:";
    for (int i = 0; i < images.size(); ++i) {
        qDebug() << QString("[%1] %2 - %3")
                    .arg(i + 1)
                    .arg(images[i].getId().left(8))
                    .arg(images[i].getFilename());
    }

    qDebug() << "";
    qDebug() << "删除模式:";
    qDebug() << "  1. 删除最后 N 张图片";
    qDebug() << "  2. 删除指定索引范围的图片";
    qDebug() << "  3. 按标签删除 (删除带有指定标签的所有图片)";
    
    QString mode = readLine("请选择模式 (1-3)");
    
    QStringList idsToDelete;
    
    if (mode == "1") {
        int n = readInt("要删除最后几张图片", 2);
        n = qBound(1, n, images.size());
        for (int i = images.size() - n; i < images.size(); ++i) {
            idsToDelete << images[i].getId();
        }
    } else if (mode == "2") {
        int start = readInt("起始索引 (1-based)", 1);
        int end = readInt("结束索引 (1-based)", qMin(2, images.size()));
        start = qBound(1, start, images.size());
        end = qBound(start, end, images.size());
        for (int i = start - 1; i < end; ++i) {
            idsToDelete << images[i].getId();
        }
    } else if (mode == "3") {
        QString tag = readLine("请输入要删除的标签 (删除所有带此标签的图片)");
        if (!tag.isEmpty()) {
            QList<ImageModel> taggedImages = repo.searchByTag(tag);
            for (const auto& img : taggedImages) {
                idsToDelete << img.getId();
            }
            qDebug() << "找到" << taggedImages.size() << "张带有标签 '" << tag << "' 的图片";
        }
    }

    if (idsToDelete.isEmpty()) {
        qDebug() << "未选择要删除的图片";
        waitForEnter();
        return;
    }

    qDebug() << "";
    qDebug() << "将要删除以下" << idsToDelete.size() << "张图片:";
    for (const QString& id : idsToDelete) {
        ImageModel model = repo.findById(id);
        qDebug() << "  -" << model.getFilename() << "(" << model.getSizeFormatted() << ")";
    }

    QString confirm = readLine("确认删除? (输入 DELETE 确认)");
    if (confirm != "DELETE") {
        qDebug() << "取消删除操作";
        waitForEnter();
        return;
    }

    int deleted = repo.removeBatch(idsToDelete);
    qDebug() << "成功删除" << deleted << "张图片";
    qDebug() << "当前剩余图片数量:" << repo.count();
    
    waitForEnter();
}
