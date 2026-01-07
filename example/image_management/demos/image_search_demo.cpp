#include "image_search_demo.h"
#include "../src/image_manager.h"
#include <QDebug>

void ImageSearchDemo::run(ImageManager& imageManager)
{
    qDebug() << "\n=== " << name() << " ===";
    qDebug() << description();
    qDebug() << "";

    // First, ensure we have some images
    QStringList allIds = imageManager.getAllImageIds();
    if (allIds.isEmpty()) {
        qDebug() << "No images found. Please run Image Upload Demo and Image Metadata Demo first.";
        return;
    }

    qDebug() << "Found" << allIds.size() << "images in the database.";

    // Demo 1: Get all image IDs
    qDebug() << "\n1. Retrieving all image IDs...";
    allIds = imageManager.getAllImageIds();
    qDebug() << "   Total images:" << allIds.size();
    qDebug() << "   First 5 IDs:";
    for (int i = 0; i < qMin(5, allIds.size()); ++i) {
        qDebug() << QString("      %1. %2...").arg(i + 1).arg(allIds[i].left(12));
    }

    // Demo 2: Search by single tag
    qDebug() << "\n2. Searching images by single tag...";
    QStringList testTags = QStringList() << "red" << "blue" << "green" << "small" << "simple";
    for (const QString& tag : testTags) {
        QStringList results = imageManager.searchByTag(tag);
        qDebug() << QString("   Tag '%1': %2 image(s) found").arg(tag).arg(results.size());
        if (!results.isEmpty()) {
            for (int i = 0; i < qMin(2, results.size()); ++i) {
                QString id = results[i];
                ImageModel metadata = imageManager.getImageMetadata(id);
                qDebug() << QString("      - %1").arg(metadata.getFilename());
            }
        }
    }

    // Demo 3: Search by all tags (AND operation)
    qDebug() << "\n3. Searching images with ALL tags (AND operation)...";
    QStringList andTags = QStringList() << "small" << "red";
    QStringList andResults = imageManager.searchByAllTags(andTags);
    qDebug() << QString("   Tags: [%1]").arg(andTags.join(", "));
    qDebug() << "   Found" << andResults.size() << "image(s):";
    for (const QString& id : andResults) {
        ImageModel metadata = imageManager.getImageMetadata(id);
        qDebug() << QString("      - %1 (tags: %2)").arg(metadata.getFilename()).arg(metadata.getTags().join(", "));
    }

    // Demo 4: Search by any tag (OR operation)
    qDebug() << "\n4. Searching images with ANY tags (OR operation)...";
    QStringList orTags = QStringList() << "red" << "blue" << "green";
    QStringList orResults = imageManager.searchByAnyTag(orTags);
    qDebug() << QString("   Tags: [%1]").arg(orTags.join(", "));
    qDebug() << "   Found" << orResults.size() << "image(s):";
    for (int i = 0; i < qMin(3, orResults.size()); ++i) {
        QString id = orResults[i];
        ImageModel metadata = imageManager.getImageMetadata(id);
        qDebug() << QString("      - %1 (tags: %2)").arg(metadata.getFilename()).arg(metadata.getTags().join(", "));
    }

    // Demo 5: Pagination
    qDebug() << "\n5. Demonstrating pagination...";
    int pageSize = 2;
    int currentPage = 0;
    int totalPages = (allIds.size() + pageSize - 1) / pageSize;

    qDebug() << QString("   Page size: %1, Total pages: %2").arg(pageSize).arg(totalPages);

    for (int page = 0; page < qMin(2, totalPages); ++page) {
        int offset = page * pageSize;
        QStringList pageIds = imageManager.getImageIdsPaginated(offset, pageSize);
        qDebug() << QString("   Page %1 (offset %2, %3 images):")
                    .arg(page + 1)
                    .arg(offset)
                    .arg(pageIds.size());
        for (const QString& id : pageIds) {
            ImageModel metadata = imageManager.getImageMetadata(id);
            qDebug() << QString("      - %1 (%2)")
                        .arg(metadata.getFilename())
                        .arg(metadata.getSizeFormatted());
        }
    }

    // Demo 6: Search non-existent tag
    qDebug() << "\n6. Searching for non-existent tag...";
    QString nonExistentTag = "nonexistenttag";
    QStringList noResults = imageManager.searchByTag(nonExistentTag);
    qDebug() << QString("   Tag '%1': %2 image(s) found").arg(nonExistentTag).arg(noResults.size());

    // Demo 7: Display detailed information for search results
    qDebug() << "\n7. Displaying detailed information for search results...";
    QString searchTag = "small";
    QStringList searchResults = imageManager.searchByTag(searchTag);
    if (!searchResults.isEmpty()) {
        qDebug() << QString("   Found %1 images tagged with '%2':").arg(searchResults.size()).arg(searchTag);
        for (int i = 0; i < searchResults.size(); ++i) {
            QString id = searchResults[i];
            ImageModel metadata = imageManager.getImageMetadata(id);
            qDebug() << QString("      [%1] %2")
                        .arg(i + 1)
                        .arg(metadata.toString());
        }
    }

    // Demo 8: Statistics
    qDebug() << "\n8. Database statistics:";
    qDebug() << "   " << imageManager.getStatistics();

    qDebug() << "\n" << name() << " completed!";
}
