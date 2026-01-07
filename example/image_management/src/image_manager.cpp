#include "image_manager.h"
#include "image_model.h"
#include "redismanager.h"
#include <QUuid>
#include <QImageReader>
#include <QBuffer>
#include <QFileInfo>
#include <QDebug>

ImageManager::ImageManager(RedisManager& redisManager)
    : m_redisManager(redisManager)
{
}

ImageManager::~ImageManager()
{
}

QString ImageManager::uploadImage(const QString& filename,
                                   const QByteArray& imageData,
                                   const QString& mimeType,
                                   const QStringList& tags)
{
    // Generate unique ID
    QString id = generateImageId();

    if (imageData.isEmpty()) {
        qWarning() << "Cannot upload empty image data";
        return QString();
    }

    // Save image data to Redis
    QString dataKey = getImageDataKey(id);
    if (!m_redisManager.set(dataKey, QString::fromLatin1(imageData.toHex()))) {
        qWarning() << "Failed to save image data for ID:" << id;
        return QString();
    }

    // Create metadata
    ImageModel model(id, filename, mimeType, imageData.size());
    model.setTags(tags);

    // Try to get image dimensions if it's an image
    QImage image = QImage::fromData(imageData);
    if (!image.isNull()) {
        model.setWidth(image.width());
        model.setHeight(image.height());
    }

    // Save metadata
    if (!saveMetadata(id, model)) {
        qWarning() << "Failed to save metadata for ID:" << id;
        // Clean up image data
        m_redisManager.del(dataKey);
        return QString();
    }

    // Update tag indexes
    updateTagIndex(id, QStringList(), tags);

    // Add to global set
    addToGlobalSet(id);

    // Update statistics
    updateStatistics(1, imageData.size());

    qDebug() << "Image uploaded successfully:" << model.toString();
    return id;
}

QString ImageManager::uploadImageFromQImage(const QString& filename,
                                             const QImage& image,
                                             const QString& format,
                                             const QStringList& tags)
{
    if (image.isNull()) {
        qWarning() << "Cannot upload null QImage";
        return QString();
    }

    // Convert QImage to QByteArray
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);

    if (!image.save(&buffer, format.toLatin1().constData())) {
        qWarning() << "Failed to convert QImage to format:" << format;
        return QString();
    }

    buffer.close();

    // Determine MIME type
    QString mimeType = "image/" + format.toLower();
    if (format.toUpper() == "JPG") {
        mimeType = "image/jpeg";
    }

    return uploadImage(filename, imageData, mimeType, tags);
}

QString ImageManager::uploadImageFromFile(const QString& filePath,
                                          const QStringList& tags)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qWarning() << "File does not exist:" << filePath;
        return QString();
    }

    // Read file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return QString();
    }

    QByteArray imageData = file.readAll();
    file.close();

    if (imageData.isEmpty()) {
        qWarning() << "File is empty:" << filePath;
        return QString();
    }

    // Get MIME type
    QString mimeType = getMimeTypeFromFilePath(filePath);

    return uploadImage(fileInfo.fileName(), imageData, mimeType, tags);
}

QByteArray ImageManager::getImageData(const QString& id)
{
    QString dataKey = getImageDataKey(id);
    QString hexData;

    hexData = m_redisManager.get(dataKey);
    if (hexData.isEmpty()) {
        qWarning() << "Failed to retrieve image data for ID:" << id;
        return QByteArray();
    }

    return QByteArray::fromHex(hexData.toLatin1());
}

QImage ImageManager::getImageAsQImage(const QString& id)
{
    QByteArray data = getImageData(id);
    if (data.isEmpty()) {
        return QImage();
    }

    return QImage::fromData(data);
}

ImageModel ImageManager::getImageMetadata(const QString& id)
{
    QString metaKey = getMetadataKey(id);
    QMap<QString, QString> fields = m_redisManager.hGetAll(metaKey);

    if (fields.isEmpty()) {
        qWarning() << "Failed to retrieve metadata for ID:" << id;
        return ImageModel();
    }

    // Convert QMap to QVariantMap
    QVariantMap variantMap;
    for (auto it = fields.begin(); it != fields.end(); ++it) {
        variantMap[it.key()] = it.value();
    }

    // Use ImageModel's deserialization
    return ImageModel::fromVariantMap(variantMap);
}

bool ImageManager::saveImageToFile(const QString& id, const QString& filepath)
{
    QByteArray data = getImageData(id);
    if (data.isEmpty()) {
        return false;
    }

    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create file:" << filepath;
        return false;
    }

    file.write(data);
    file.close();

    qDebug() << "Image saved to file:" << filepath;
    return true;
}

bool ImageManager::updateMetadata(const QString& id, const ImageModel& metadata)
{
    if (!imageExists(id)) {
        qWarning() << "Cannot update metadata for non-existent image:" << id;
        return false;
    }

    return saveMetadata(id, metadata);
}

bool ImageManager::updateTags(const QString& id, const QStringList& tags)
{
    ImageModel metadata = getImageMetadata(id);
    if (metadata.getId().isEmpty()) {
        qWarning() << "Failed to get metadata for image:" << id;
        return false;
    }

    QStringList oldTags = metadata.getTags();
    metadata.setTags(tags);

    // Update tag indexes
    updateTagIndex(id, oldTags, tags);

    // Save updated metadata
    return saveMetadata(id, metadata);
}

bool ImageManager::addTagToImage(const QString& id, const QString& tag)
{
    ImageModel metadata = getImageMetadata(id);
    if (metadata.getId().isEmpty()) {
        return false;
    }

    if (metadata.hasTag(tag)) {
        qDebug() << "Tag already exists:" << tag;
        return true;
    }

    metadata.addTag(tag);

    // Add to tag index
    QString tagSetKey = getTagSetKey(tag);
    m_redisManager.sAdd(tagSetKey, id);

    return saveMetadata(id, metadata);
}

bool ImageManager::removeTagFromImage(const QString& id, const QString& tag)
{
    ImageModel metadata = getImageMetadata(id);
    if (metadata.getId().isEmpty()) {
        return false;
    }

    if (!metadata.hasTag(tag)) {
        qDebug() << "Tag does not exist:" << tag;
        return true;
    }

    metadata.removeTag(tag);

    // Remove from tag index
    QString tagSetKey = getTagSetKey(tag);
    m_redisManager.sRem(tagSetKey, id);

    return saveMetadata(id, metadata);
}

QStringList ImageManager::getAllImageIds()
{
    QString allIdsKey = getAllIdsKey();
    QVector<QString> ids = m_redisManager.sMembers(allIdsKey);
    return ids.toList();
}

QStringList ImageManager::searchByTag(const QString& tag)
{
    QString tagSetKey = getTagSetKey(tag);
    QVector<QString> ids = m_redisManager.sMembers(tagSetKey);

    return ids.toList();
}

QStringList ImageManager::searchByAllTags(const QStringList& tags)
{
    if (tags.isEmpty()) {
        return getAllImageIds();
    }

    // Get IDs for first tag
    QStringList result = searchByTag(tags.first());

    // Intersect with IDs for other tags
    for (int i = 1; i < tags.size(); ++i) {
        QString tag = tags[i];
        QStringList tagIds = searchByTag(tag);
        result = QSet<QString>(result.begin(), result.end())
                  .intersect(QSet<QString>(tagIds.begin(), tagIds.end()))
                  .values();
    }

    return result;
}

QStringList ImageManager::searchByAnyTag(const QStringList& tags)
{
    QSet<QString> result;

    for (const QString& tag : tags) {
        result.unite(QSet<QString>(searchByTag(tag).begin(), searchByTag(tag).end()));
    }

    return result.values();
}

QStringList ImageManager::getImageIdsPaginated(int offset, int limit)
{
    QStringList allIds = getAllImageIds();

    int start = qMax(0, offset);
    int end = qMin(allIds.size(), offset + limit);

    if (start >= allIds.size()) {
        return QStringList();
    }

    return allIds.mid(start, end - start);
}

bool ImageManager::deleteImage(const QString& id)
{
    if (!imageExists(id)) {
        qWarning() << "Image does not exist:" << id;
        return false;
    }

    // Get metadata before deletion
    ImageModel metadata = getImageMetadata(id);

    // Delete image data
    QString dataKey = getImageDataKey(id);
    m_redisManager.del(dataKey);

    // Delete metadata
    QString metaKey = getMetadataKey(id);
    m_redisManager.del(metaKey);

    // Remove from tag indexes
    updateTagIndex(id, metadata.getTags(), QStringList());

    // Remove from global set
    removeFromGlobalSet(id);

    // Update statistics
    updateStatistics(-1, -metadata.getSize());

    qDebug() << "Image deleted successfully:" << id;
    return true;
}

int ImageManager::deleteImages(const QStringList& ids)
{
    int deletedCount = 0;

    for (const QString& id : ids) {
        if (deleteImage(id)) {
            deletedCount++;
        }
    }

    return deletedCount;
}

bool ImageManager::clearAllImages()
{
    QStringList allIds = getAllImageIds();
    int deleted = deleteImages(allIds);

    qDebug() << "Cleared" << deleted << "images";
    return deleted == allIds.size();
}

int ImageManager::getImageCount()
{
    QString allIdsKey = getAllIdsKey();
    return m_redisManager.sCard(allIdsKey);
}

qint64 ImageManager::getTotalSize()
{
    QString statsKey = getStatsKey();
    QString sizeStr = m_redisManager.hGet(statsKey, "total_size");

    if (sizeStr.isEmpty()) {
        return 0;
    }

    return sizeStr.toLongLong();
}

QString ImageManager::getStatistics()
{
    int count = getImageCount();
    qint64 totalSize = getTotalSize();

    QString sizeStr;
    if (totalSize < 1024) {
        sizeStr = QString("%1 B").arg(totalSize);
    } else if (totalSize < 1024 * 1024) {
        sizeStr = QString("%1 KB").arg(totalSize / 1024.0, 0, 'f', 2);
    } else if (totalSize < 1024 * 1024 * 1024) {
        sizeStr = QString("%1 MB").arg(totalSize / (1024.0 * 1024.0), 0, 'f', 2);
    } else {
        sizeStr = QString("%1 GB").arg(totalSize / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }

    return QString("Total Images: %1, Total Size: %2").arg(count).arg(sizeStr);
}

bool ImageManager::imageExists(const QString& id)
{
    QString dataKey = getImageDataKey(id);
    return m_redisManager.exists(dataKey);
}

QString ImageManager::generateImageId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString ImageManager::getMimeTypeFromFilePath(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    return getMimeTypeFromExtension(fileInfo.suffix());
}

QString ImageManager::getMimeTypeFromExtension(const QString& extension)
{
    QString ext = extension.toLower();

    if (ext == "jpg" || ext == "jpeg") {
        return "image/jpeg";
    } else if (ext == "png") {
        return "image/png";
    } else if (ext == "gif") {
        return "image/gif";
    } else if (ext == "bmp") {
        return "image/bmp";
    } else if (ext == "webp") {
        return "image/webp";
    } else if (ext == "tiff" || ext == "tif") {
        return "image/tiff";
    } else {
        return "application/octet-stream";
    }
}

QString ImageManager::getImageDataKey(const QString& id) const
{
    return QString("image:%1").arg(id);
}

QString ImageManager::getMetadataKey(const QString& id) const
{
    return QString("image:meta:%1").arg(id);
}

QString ImageManager::getTagSetKey(const QString& tag) const
{
    return QString("image:tags:%1").arg(tag);
}

QString ImageManager::getAllIdsKey() const
{
    return "image:all_ids";
}

QString ImageManager::getStatsKey() const
{
    return "image:stats";
}

bool ImageManager::saveMetadata(const QString& id, const ImageModel& metadata)
{
    QString metaKey = getMetadataKey(id);

    // Convert ImageModel to QVariantMap
    QVariantMap variantMap = metadata.toVariantMap();

    // Save each field to Redis hash
    for (auto it = variantMap.begin(); it != variantMap.end(); ++it) {
        if (!m_redisManager.hSet(metaKey, it.key(), it.value().toString())) {
            qWarning() << "Failed to set field" << it.key() << "for metadata:" << id;
            return false;
        }
    }

    return true;
}

bool ImageManager::updateTagIndex(const QString& id, const QStringList& oldTags, const QStringList& newTags)
{
    // Remove from old tag sets
    for (const QString& tag : oldTags) {
        if (!newTags.contains(tag)) {
            QString tagSetKey = getTagSetKey(tag);
            m_redisManager.sRem(tagSetKey, id);
        }
    }

    // Add to new tag sets
    for (const QString& tag : newTags) {
        if (!oldTags.contains(tag)) {
            QString tagSetKey = getTagSetKey(tag);
            m_redisManager.sAdd(tagSetKey, id);
        }
    }

    return true;
}

bool ImageManager::addToGlobalSet(const QString& id)
{
    QString allIdsKey = getAllIdsKey();
    return m_redisManager.sAdd(allIdsKey, id);
}

bool ImageManager::removeFromGlobalSet(const QString& id)
{
    QString allIdsKey = getAllIdsKey();
    return m_redisManager.sRem(allIdsKey, id);
}

bool ImageManager::updateStatistics(int countChange, qint64 sizeChange)
{
    QString statsKey = getStatsKey();

    // Get current count
    int currentCount = getImageCount();
    int newCount = currentCount + countChange;

    // Update count field
    m_redisManager.hSet(statsKey, "total_count", QString::number(newCount));

    // Update total size
    qint64 currentSize = getTotalSize();
    qint64 newSize = currentSize + sizeChange;
    m_redisManager.hSet(statsKey, "total_size", QString::number(newSize));

    return true;
}
