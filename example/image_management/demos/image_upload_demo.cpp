#include "image_upload_demo.h"
#include "../src/image_manager.h"
#include "../src/image_model.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>

void ImageUploadDemo::run(ImageManager& imageManager)
{
    qDebug() << "\n=== " << name() << " ===";
    qDebug() << description();
    qDebug() << "";

    // Demo 1: Create and upload a simple image from QImage
    qDebug() << "1. Creating and uploading a simple red square (100x100)...";
    QImage redSquare(100, 100, QImage::Format_RGB32);
    redSquare.fill(Qt::red);

    QString id1 = imageManager.uploadImageFromQImage("red_square.png", redSquare, "PNG", QStringList() << "simple" << "red");
    if (!id1.isEmpty()) {
        qDebug() << "   成功! 图片 ID:" << id1;
        ImageModel metadata = imageManager.getImageMetadata(id1);
        qDebug() << "   Metadata:" << metadata.toString();
    } else {
        qWarning() << "   Failed to upload red square";
    }

    // Demo 2: Create and upload a gradient image
    qDebug() << "\n2. Creating and uploading a gradient image (200x100)...";
    QImage gradientImage(200, 100, QImage::Format_RGB32);
    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 200; ++x) {
            int r = (x * 255) / 200;
            int g = (y * 255) / 100;
            int b = 128;
            gradientImage.setPixelColor(x, y, QColor(r, g, b));
        }
    }

    QString id2 = imageManager.uploadImageFromQImage("gradient.png", gradientImage, "PNG", QStringList() << "gradient" << "color");
    if (!id2.isEmpty()) {
        qDebug() << "   成功! 图片 ID:" << id2;
        ImageModel metadata = imageManager.getImageMetadata(id2);
        qDebug() << "   Metadata:" << metadata.toString();
    } else {
        qWarning() << "   Failed to upload gradient image";
    }

    // Demo 3: Upload a checkerboard pattern image
    qDebug() << "\n3. Creating and uploading a checkerboard pattern (150x150)...";
    QImage checkerboard(150, 150, QImage::Format_RGB32);
    const int cellSize = 25;
    for (int y = 0; y < 150; ++y) {
        for (int x = 0; x < 150; ++x) {
            bool black = ((x / cellSize + y / cellSize) % 2) == 0;
            checkerboard.setPixelColor(x, y, black ? Qt::black : Qt::white);
        }
    }

    QString id3 = imageManager.uploadImageFromQImage("checkerboard.png", checkerboard, "PNG", QStringList() << "pattern" << "blackwhite");
    if (!id3.isEmpty()) {
        qDebug() << "   成功! 图片 ID:" << id3;
        ImageModel metadata = imageManager.getImageMetadata(id3);
        qDebug() << "   Metadata:" << metadata.toString();
    } else {
        qWarning() << "   Failed to upload checkerboard";
    }

    // Demo 4: Upload from raw QByteArray
    qDebug() << "\n4. Uploading image from raw QByteArray...";
    QImage blueRect(80, 120, QImage::Format_RGB32);
    blueRect.fill(Qt::blue);

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    blueRect.save(&buffer, "PNG");
    buffer.close();

    QString id4 = imageManager.uploadImage("blue_rect.png", imageData, "image/png", QStringList() << "simple" << "blue");
    if (!id4.isEmpty()) {
        qDebug() << "   成功! 图片 ID:" << id4;
        ImageModel metadata = imageManager.getImageMetadata(id4);
        qDebug() << "   Metadata:" << metadata.toString();
    } else {
        qWarning() << "   Failed to upload from raw data";
    }

    // Summary
    qDebug() << "\n5. Summary:";
    int count = imageManager.getImageCount();
    qDebug() << "   总共上传的图片:" << count;
    qDebug() << "   Statistics:" << imageManager.getStatistics();

    qDebug() << "\n" << name() << " completed!";
}
