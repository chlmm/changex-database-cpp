#include "image_retrieve_demo.h"
#include "../src/image_manager.h"
#include "../src/image_model.h"
#include <QImage>
#include <QFile>
#include <QDebug>

void ImageRetrieveDemo::run(ImageManager& imageManager)
{
    qDebug() << "\n=== " << name() << " ===";
    qDebug() << description();
    qDebug() << "";

    // First, ensure we have some images
    QStringList allIds = imageManager.getAllImageIds();
    if (allIds.isEmpty()) {
        qDebug() << "No images found. Please run Image Upload Demo first to create some images.";
        return;
    }

    qDebug() << "找到" << allIds.size() << "个图片在数据库中。";

    // Demo 1: Retrieve image data as QByteArray
    qDebug() << "\n1. Retrieving image data as QByteArray...";
    QString id1 = allIds.first();
    QByteArray imageData = imageManager.getImageData(id1);
    if (!imageData.isEmpty()) {
        qDebug() << "   成功! 获取了" << imageData.size() << "字节";
        ImageModel metadata = imageManager.getImageMetadata(id1);
        qDebug() << "   Image:" << metadata.toString();
    } else {
        qWarning() << "   Failed to retrieve image data";
    }

    // Demo 2: Retrieve image as QImage
    qDebug() << "\n2. Retrieving image as QImage object...";
    QImage image = imageManager.getImageAsQImage(id1);
    if (!image.isNull()) {
        qDebug() << "   成功! 图片尺寸:" << image.width() << "x" << image.height();
        qDebug() << "   Format:" << image.format();
    } else {
        qWarning() << "   Failed to retrieve QImage";
    }

    // Demo 3: Get metadata for all images
    qDebug() << "\n3. Retrieving metadata for all images...";
    for (int i = 0; i < allIds.size() && i < 3; ++i) {
        QString id = allIds[i];
        ImageModel metadata = imageManager.getImageMetadata(id);
        qDebug() << QString("   [%1] ").arg(i + 1) << metadata.toString();
    }

    // Demo 4: Save image to file
    qDebug() << "\n4. Saving image to local file...";
    QString savePath = "/tmp/test_image.png";
    if (imageManager.saveImageToFile(id1, savePath)) {
        qDebug() << "   成功! 图片已保存到:" << savePath;
        qDebug() << "   File exists:" << QFile::exists(savePath);
    } else {
        qWarning() << "   Failed to save image to file";
    }

    // Demo 5: Retrieve multiple images with pagination
    qDebug() << "\n5. Retrieving images with pagination...";
    int pageSize = 2;
    QStringList firstPage = imageManager.getImageIdsPaginated(0, pageSize);
    qDebug() << "   First page (" << firstPage.size() << "images):";
    for (const QString& id : firstPage) {
        ImageModel metadata = imageManager.getImageMetadata(id);
        qDebug() << "      -" << metadata.getFilename() << "(" << metadata.getSizeFormatted() << ")";
    }

    if (allIds.size() > pageSize) {
        QStringList secondPage = imageManager.getImageIdsPaginated(pageSize, pageSize);
        qDebug() << "   Second page (" << secondPage.size() << "images):";
        for (const QString& id : secondPage) {
            ImageModel metadata = imageManager.getImageMetadata(id);
            qDebug() << "      -" << metadata.getFilename() << "(" << metadata.getSizeFormatted() << ")";
        }
    }

    // Demo 6: Check if images exist
    qDebug() << "\n6. Checking image existence...";
    for (const QString& id : allIds.mid(0, 3)) {
        bool exists = imageManager.imageExists(id);
        qDebug() << QString("   ID: %1... exists: %2")
                    .arg(id.left(8))
                    .arg(exists ? "Yes" : "No");
    }

    // Check non-existent image
    QString fakeId = "non-existent-id-12345";
    bool fakeExists = imageManager.imageExists(fakeId);
    qDebug() << QString("   Fake ID %1... exists: %2")
                .arg(fakeId)
                .arg(fakeExists ? "Yes (unexpected!)" : "No");

    qDebug() << "\n" << name() << " completed!";
}
