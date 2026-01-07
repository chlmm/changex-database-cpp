#include "image_model_demo.h"
#include "../src/image_manager.h"
#include <QDebug>

void ImageModelDemo::run(ImageManager& imageManager)
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

    QString targetId = allIds.first();
    qDebug() << "Using image ID:" << targetId;

    // Demo 1: Get and display current metadata
    qDebug() << "\n1. Retrieving current metadata...";
    ImageModel metadata = imageManager.getImageMetadata(targetId);
    qDebug() << "   Current metadata:" << metadata.toString();
    qDebug() << "   Tags:" << metadata.getTags().join(", ");

    // Demo 2: Add tags to an image
    qDebug() << "\n2. 添加标签到图片...";
    QStringList newTags = QStringList() << "test" << "demo" << "featured";
    for (const QString& tag : newTags) {
        if (imageManager.addTagToImage(targetId, tag)) {
            qDebug() << QString("   Successfully added tag: '%1'").arg(tag);
        } else {
            qWarning() << QString("   Failed to add tag: '%1'").arg(tag);
        }
    }

    // Refresh metadata
    metadata = imageManager.getImageMetadata(targetId);
    qDebug() << "   Updated tags:" << metadata.getTags().join(", ");

    // Demo 3: Remove tags from an image
    qDebug() << "\n3. 从图片中移除标签...";
    QStringList tagsToRemove = QStringList() << "test" << "demo";
    for (const QString& tag : tagsToRemove) {
        if (imageManager.removeTagFromImage(targetId, tag)) {
            qDebug() << QString("   Successfully removed tag: '%1'").arg(tag);
        } else {
            qWarning() << QString("   Failed to remove tag: '%1'").arg(tag);
        }
    }

    // Refresh metadata
    metadata = imageManager.getImageMetadata(targetId);
    qDebug() << "   Updated tags:" << metadata.getTags().join(", ");

    // Demo 4: Update all tags at once
    qDebug() << "\n4. 一次性更新所有标签...";
    QStringList updatedTags = QStringList() << "landscape" << "nature" << "scenery";
    if (imageManager.updateTags(targetId, updatedTags)) {
        qDebug() << "   成功! 新标签:" << updatedTags.join(", ");
    } else {
        qWarning() << "   Failed to update tags";
    }

    // Refresh metadata
    metadata = imageManager.getImageMetadata(targetId);
    qDebug() << "   Current tags:" << metadata.getTags().join(", ");

    // Demo 5: Update complete metadata
    qDebug() << "\n5. 更新完整的元数据...";
    metadata.setFilename("updated_" + metadata.getFilename());
    metadata.setWidth(metadata.getWidth() + 10);
    metadata.setHeight(metadata.getHeight() + 10);

    if (imageManager.updateMetadata(targetId, metadata)) {
        qDebug() << "   成功!";
        qDebug() << "   New metadata:" << metadata.toString();
    } else {
        qWarning() << "   Failed to update metadata";
    }

    // Demo 6: Verify the update
    qDebug() << "\n6. Verifying the update...";
    ImageModel updated = imageManager.getImageMetadata(targetId);
    qDebug() << "   Filename:" << updated.getFilename();
    qDebug() << "   Dimensions:" << updated.getWidth() << "x" << updated.getHeight();
    qDebug() << "   Tags:" << updated.getTags().join(", ");

    // Demo 7: Add multiple images with various tags for demonstration
    qDebug() << "\n7. Creating additional images with different tags...";
    QImage testImage1(50, 50, QImage::Format_RGB32);
    testImage1.fill(Qt::red);
    QString id1 = imageManager.uploadImageFromQImage("image1.png", testImage1, "PNG", QStringList() << "red" << "small");

    QImage testImage2(50, 50, QImage::Format_RGB32);
    testImage2.fill(Qt::blue);
    QString id2 = imageManager.uploadImageFromQImage("image2.png", testImage2, "PNG", QStringList() << "blue" << "small");

    QImage testImage3(50, 50, QImage::Format_RGB32);
    testImage3.fill(Qt::green);
    QString id3 = imageManager.uploadImageFromQImage("image3.png", testImage3, "PNG", QStringList() << "green" << "small");

    qDebug() << "   Created 3 additional test images:";
    if (!id1.isEmpty()) qDebug() << "      - Image 1 ID:" << id1;
    if (!id2.isEmpty()) qDebug() << "      - Image 2 ID:" << id2;
    if (!id3.isEmpty()) qDebug() << "      - Image 3 ID:" << id3;

    // Demo 8: Display metadata for multiple images
    qDebug() << "\n8. 显示所有图片的元数据...";
    allIds = imageManager.getAllImageIds();
    for (int i = 0; i < allIds.size() && i < 5; ++i) {
        QString id = allIds[i];
        ImageModel meta = imageManager.getImageMetadata(id);
        qDebug() << QString("   [%1] Filename: %2, Tags: %3, Size: %4")
                    .arg(i + 1)
                    .arg(meta.getFilename())
                    .arg(meta.getTags().join(", "))
                    .arg(meta.getSizeFormatted());
    }

    qDebug() << "\n" << name() << " completed!";
}
