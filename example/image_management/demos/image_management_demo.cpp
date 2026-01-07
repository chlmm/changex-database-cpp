#include "image_management_demo.h"
#include "../src/image_manager.h"
#include "../src/image_model.h"
#include <QPainter>
#include <QFile>
#include <QDebug>

void ImageManagementDemo::run(ImageManager& imageManager)
{
    qDebug() << "\n=== " << name() << " ===";
    qDebug() << description();
    qDebug() << "";

    // Step 1: Clear any existing images
    qDebug() << "Step 1: Clearing any existing images...";
    if (imageManager.clearAllImages()) {
        qDebug() << "   Database cleared successfully.";
    }

    // Step 2: Upload multiple images with different tags
    qDebug() << "\nStep 2: Uploading images...";
    QMap<QString, QStringList> imagesToUpload;
    imagesToUpload["landscape.jpg"] = QStringList() << "landscape" << "nature";
    imagesToUpload["portrait.jpg"] = QStringList() << "portrait" << "people";
    imagesToUpload["abstract.jpg"] = QStringList() << "abstract" << "art";

    QStringList uploadedIds;
    QMap<QString, QImage> testImages;

    // Create test images
    QImage landscape(300, 200, QImage::Format_RGB32);
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 300; ++x) {
            int g = 100 + (y * 155) / 200;
            landscape.setPixelColor(x, y, QColor(0, g, 0));
        }
    }
    testImages["landscape.jpg"] = landscape;

    QImage portrait(200, 300, QImage::Format_RGB32);
    portrait.fill(Qt::yellow);
    testImages["portrait.jpg"] = portrait;

    QImage abstract(250, 250, QImage::Format_RGB32);
    for (int y = 0; y < 250; ++y) {
        for (int x = 0; x < 250; ++x) {
            int r = (x * 255) / 250;
            int b = (y * 255) / 250;
            abstract.setPixelColor(x, y, QColor(r, 100, b));
        }
    }
    testImages["abstract.jpg"] = abstract;

    // Upload images
    for (auto it = imagesToUpload.constBegin(); it != imagesToUpload.constEnd(); ++it) {
        QString filename = it.key();
        QStringList tags = it.value();
        QImage& image = testImages[filename];

        QString id = imageManager.uploadImageFromQImage(filename, image, "JPG", tags);
        if (!id.isEmpty()) {
            uploadedIds.append(id);
            qDebug() << QString("   Uploaded '%1' with tags: %2").arg(filename).arg(tags.join(", "));
        }
    }

    // Step 3: Upload additional images for testing
    qDebug() << "\nStep 3: Uploading additional images for testing...";
    QImage redCircle(150, 150, QImage::Format_RGB32);
    redCircle.fill(Qt::white);
    QPainter painter(&redCircle);
    painter.setBrush(Qt::red);
    painter.drawEllipse(0, 0, 150, 150);
    painter.end();
    QString id4 = imageManager.uploadImageFromQImage("red_circle.jpg", redCircle, "JPG", QStringList() << "red" << "circle");
    if (!id4.isEmpty()) uploadedIds.append(id4);

    QImage blueSquare(150, 150, QImage::Format_RGB32);
    blueSquare.fill(Qt::blue);
    QString id5 = imageManager.uploadImageFromQImage("blue_square.jpg", blueSquare, "JPG", QStringList() << "blue" << "square");
    if (!id5.isEmpty()) uploadedIds.append(id5);

    qDebug() << "   Total images uploaded:" << uploadedIds.size();

    // Step 4: Display all uploaded images
    qDebug() << "\nStep 4: Displaying all uploaded images...";
    QStringList allIds = imageManager.getAllImageIds();
    qDebug() << "   Found" << allIds.size() << "images:";
    for (int i = 0; i < allIds.size(); ++i) {
        QString id = allIds[i];
        ImageModel metadata = imageManager.getImageMetadata(id);
        qDebug() << QString("      [%1] %2 (%3, %4x%5, tags: %6)")
                    .arg(i + 1)
                    .arg(metadata.getFilename())
                    .arg(metadata.getSizeFormatted())
                    .arg(metadata.getWidth())
                    .arg(metadata.getHeight())
                    .arg(metadata.getTags().join(", "));
    }

    // Step 5: Search images by tag
    qDebug() << "\nStep 5: Searching images by tag...";
    QStringList searchTags = QStringList() << "landscape" << "portrait" << "abstract";
    for (const QString& tag : searchTags) {
        QStringList results = imageManager.searchByTag(tag);
        qDebug() << QString("   Tag '%1': %2 image(s)").arg(tag).arg(results.size());
        for (const QString& id : results) {
            ImageModel metadata = imageManager.getImageMetadata(id);
            qDebug() << QString("      - %1").arg(metadata.getFilename());
        }
    }

    // Step 6: Update image metadata
    qDebug() << "\nStep 6: Updating image metadata...";
    if (!uploadedIds.isEmpty()) {
        QString targetId = uploadedIds.first();
        ImageModel metadata = imageManager.getImageMetadata(targetId);

        qDebug() << "   Updating tags for image:" << metadata.getFilename();
        QStringList newTags = metadata.getTags();
        newTags << "featured" << "updated";
        if (imageManager.updateTags(targetId, newTags)) {
            qDebug() << "   Tags updated successfully:" << newTags.join(", ");
        }

        // Verify update
        ImageModel updated = imageManager.getImageMetadata(targetId);
        qDebug() << "   Current tags:" << updated.getTags().join(", ");
    }

    // Step 7: Retrieve and save an image
    qDebug() << "\nStep 7: Retrieving and saving an image to file...";
    if (!uploadedIds.isEmpty()) {
        QString id = uploadedIds.first();
        QString savePath = "/tmp/retrieved_image.jpg";

        if (imageManager.saveImageToFile(id, savePath)) {
            qDebug() << "   Image saved successfully to:" << savePath;
            qDebug() << "   File exists:" << QFile::exists(savePath);

            // Verify the saved file
            QFile file(savePath);
            qDebug() << "   File size:" << file.size() << "bytes";
        }
    }

    // Step 8: Search with multiple tags
    qDebug() << "\nStep 8: Searching with multiple tags...";
    QStringList multiTags = QStringList() << "red" << "blue";
    QStringList orResults = imageManager.searchByAnyTag(multiTags);
    qDebug() << QString("   Images with ANY of tags [%1]: %2 found")
                .arg(multiTags.join(", "))
                .arg(orResults.size());

    QStringList andTags = QStringList() << "landscape" << "nature";
    QStringList andResults = imageManager.searchByAllTags(andTags);
    qDebug() << QString("   Images with ALL of tags [%1]: %2 found")
                .arg(andTags.join(", "))
                .arg(andResults.size());

    // Step 9: Display statistics
    qDebug() << "\nStep 9: Displaying database statistics...";
    qDebug() << "   " << imageManager.getStatistics();
    qDebug() << QString("   Total image count: %1").arg(imageManager.getImageCount());
    qDebug() << QString("   Total storage size: %1 bytes").arg(imageManager.getTotalSize());

    // Step 10: Delete specific images
    qDebug() << "\nStep 10: Deleting specific images...";
    if (uploadedIds.size() >= 2) {
        QString idToDelete = uploadedIds.takeLast();
        ImageModel metadata = imageManager.getImageMetadata(idToDelete);

        qDebug() << "   Deleting image:" << metadata.getFilename();
        if (imageManager.deleteImage(idToDelete)) {
            qDebug() << "   Image deleted successfully.";
        }

        qDebug() << QString("   Remaining images: %1").arg(imageManager.getImageCount());
    }

    // Step 11: Final status
    qDebug() << "\nStep 11: Final status...";
    allIds = imageManager.getAllImageIds();
    qDebug() << "   Total images in database:" << allIds.size();
    qDebug() << "   " << imageManager.getStatistics();

    qDebug() << "\n" << name() << " completed successfully!";
    qDebug() << "All operations demonstrated: upload, retrieve, update, search, and delete images.";
}
