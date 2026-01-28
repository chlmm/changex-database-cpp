#include "image_repository.h"
#include <RedisModule/redismanager.h>
#include <QUuid>
#include <QImageReader>
#include <QBuffer>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include <QSet>

// ============ 构造函数与析构函数 ============

ImageRepository::ImageRepository(RedisManager& redisManager)
    : m_redis(redisManager)
{
}

ImageRepository::~ImageRepository()
{
}

// ============ 增删改查 ============

QString ImageRepository::create(const ImageModel& model, const QByteArray& imageData)
{
    if (imageData.isEmpty() || !model.isValid()) {
        qWarning() << "Cannot create image: empty data or invalid model";
        return QString();
    }

    QString id = generateId();

    // 保存图片数据
    QString dataKey = keyImageData(id);
    if (!m_redis.set(dataKey, QString::fromLatin1(imageData.toHex()))) {
        qWarning() << "Failed to save image data for ID:" << id;
        return QString();
    }

    // 准备并保存元数据
    ImageModel newModel = model;
    newModel.setId(id);
    newModel.setSize(imageData.size());

    if (!saveMetadata(id, newModel)) {
        qWarning() << "Failed to save metadata for ID:" << id;
        m_redis.del(dataKey);
        return QString();
    }

    // 更新标签索引
    updateTagIndex(id, QStringList(), newModel.getTags());

    // 添加到全局集合
    addToGlobalSet(id);

    // 更新统计
    updateStatistics(1, imageData.size());

    qDebug() << "Image created successfully:" << newModel.toString();
    return id;
}

ImageModel ImageRepository::findById(const QString& id)
{
    QString metaKey = keyImageMeta(id);
    QMap<QString, QString> fields = m_redis.hGetAll(metaKey);

    if (fields.isEmpty()) {
        return ImageModel();
    }

    QVariantMap variantMap;
    for (auto it = fields.begin(); it != fields.end(); ++it) {
        variantMap[it.key()] = it.value();
    }

    return ImageModel::fromVariantMap(variantMap);
}

QByteArray ImageRepository::getImageData(const QString& id)
{
    QString dataKey = keyImageData(id);
    QString hexData = m_redis.get(dataKey);
    
    if (hexData.isEmpty()) {
        qWarning() << "Failed to retrieve image data for ID:" << id;
        return QByteArray();
    }

    return QByteArray::fromHex(hexData.toLatin1());
}

QImage ImageRepository::getImageAsQImage(const QString& id)
{
    QByteArray data = getImageData(id);
    if (data.isEmpty()) {
        return QImage();
    }
    return QImage::fromData(data);
}

bool ImageRepository::update(const ImageModel& model)
{
    if (!model.isValid() || !exists(model.getId())) {
        qWarning() << "Cannot update: invalid model or image does not exist";
        return false;
    }

    QString id = model.getId();
    ImageModel oldModel = findById(id);
    
    // 更新标签索引
    updateTagIndex(id, oldModel.getTags(), model.getTags());

    // 保存新元数据
    return saveMetadata(id, model);
}

bool ImageRepository::remove(const QString& id)
{
    if (!exists(id)) {
        qWarning() << "Image does not exist:" << id;
        return false;
    }

    ImageModel model = findById(id);
    qint64 size = model.getSize();

    // 删除数据
    m_redis.del(keyImageData(id));
    m_redis.del(keyImageMeta(id));

    // 更新标签索引
    updateTagIndex(id, model.getTags(), QStringList());

    // 从全局集合移除
    removeFromGlobalSet(id);

    // 更新统计
    updateStatistics(-1, -size);

    qDebug() << "Image deleted successfully:" << id;
    return true;
}

int ImageRepository::removeBatch(const QStringList& ids)
{
    int count = 0;
    for (const QString& id : ids) {
        if (remove(id)) {
            count++;
        }
    }
    return count;
}

// ============ 查询 ============

QList<ImageModel> ImageRepository::findAll()
{
    QList<ImageModel> results;
    QStringList ids = m_redis.sMembers(keyAllIds()).toList();

    for (const QString& id : ids) {
        ImageModel model = findById(id);
        if (model.isValid()) {
            results.append(model);
        }
    }

    return results;
}

QStringList ImageRepository::findIdsPaginated(int offset, int limit)
{
    QStringList allIds = m_redis.sMembers(keyAllIds()).toList();
    
    int start = qMax(0, offset);
    int end = qMin(allIds.size(), offset + limit);
    
    if (start >= allIds.size()) {
        return QStringList();
    }
    
    return allIds.mid(start, end - start);
}

QList<ImageModel> ImageRepository::searchByTag(const QString& tag)
{
    QList<ImageModel> results;
    QString tagKey = keyTagIndex(tag);
    QStringList ids = m_redis.sMembers(tagKey).toList();

    for (const QString& id : ids) {
        ImageModel model = findById(id);
        if (model.isValid()) {
            results.append(model);
        }
    }

    return results;
}

QList<ImageModel> ImageRepository::searchByTags(const QStringList& tags, bool matchAll)
{
    if (tags.isEmpty()) {
        return findAll();
    }

    QStringList resultIds;
    
    if (matchAll) {
        // AND 逻辑：从第一个标签开始交集
        resultIds = m_redis.sMembers(keyTagIndex(tags.first())).toList();
        for (int i = 1; i < tags.size() && !resultIds.isEmpty(); ++i) {
            QStringList tagIds = m_redis.sMembers(keyTagIndex(tags[i])).toList();
            QSet<QString> currentSet(resultIds.begin(), resultIds.end());
            QSet<QString> tagSet(tagIds.begin(), tagIds.end());
            resultIds = currentSet.intersect(tagSet).values();
        }
    } else {
        // OR 逻辑：并集
        QSet<QString> resultSet;
        for (const QString& tag : tags) {
            QStringList tagIds = m_redis.sMembers(keyTagIndex(tag)).toList();
            resultSet.unite(QSet<QString>(tagIds.begin(), tagIds.end()));
        }
        resultIds = resultSet.values();
    }

    QList<ImageModel> results;
    for (const QString& id : resultIds) {
        ImageModel model = findById(id);
        if (model.isValid()) {
            results.append(model);
        }
    }

    return results;
}

// ============ Tag 管理 ============

bool ImageRepository::addTag(const QString& id, const QString& tag)
{
    ImageModel model = findById(id);
    if (!model.isValid()) {
        return false;
    }

    if (model.hasTag(tag)) {
        return true;
    }

    model.addTag(tag);
    m_redis.sAdd(keyTagIndex(tag), id);
    
    return saveMetadata(id, model);
}

bool ImageRepository::removeTag(const QString& id, const QString& tag)
{
    ImageModel model = findById(id);
    if (!model.isValid()) {
        return false;
    }

    if (!model.hasTag(tag)) {
        return true;
    }

    model.removeTag(tag);
    m_redis.sRem(keyTagIndex(tag), id);
    
    return saveMetadata(id, model);
}

bool ImageRepository::updateTags(const QString& id, const QStringList& tags)
{
    ImageModel model = findById(id);
    if (!model.isValid()) {
        return false;
    }

    QStringList oldTags = model.getTags();
    model.setTags(tags);
    
    updateTagIndex(id, oldTags, tags);
    
    return saveMetadata(id, model);
}

// ============ 统计与工具 ============

int ImageRepository::count()
{
    return m_redis.sCard(keyAllIds());
}

qint64 ImageRepository::getTotalSize()
{
    QString statsKey = keyStats();
    QString sizeStr = m_redis.hGet(statsKey, "total_size");
    return sizeStr.isEmpty() ? 0 : sizeStr.toLongLong();
}

QString ImageRepository::getStatistics()
{
    int imageCount = count();
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

    return QString("Total Images: %1, Total Size: %2").arg(imageCount).arg(sizeStr);
}

bool ImageRepository::exists(const QString& id)
{
    return m_redis.exists(keyImageData(id));
}

bool ImageRepository::clearAll()
{
    QStringList allIds = m_redis.sMembers(keyAllIds()).toList();
    int deleted = removeBatch(allIds);
    qDebug() << "Cleared" << deleted << "images";
    return deleted == allIds.size();
}

bool ImageRepository::saveToFile(const QString& id, const QString& filePath)
{
    QByteArray data = getImageData(id);
    if (data.isEmpty()) {
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create file:" << filePath;
        return false;
    }

    file.write(data);
    file.close();

    qDebug() << "Image saved to file:" << filePath;
    return true;
}

// ============ 便捷上传方法 ============

QString ImageRepository::uploadFromFile(const QString& filePath, const QStringList& tags)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        qWarning() << "File does not exist:" << filePath;
        return QString();
    }

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

    QString mimeType = getMimeTypeFromExtension(fileInfo.suffix());
    
    // 尝试获取图片尺寸
    QImage image = QImage::fromData(imageData);
    int width = image.isNull() ? 0 : image.width();
    int height = image.isNull() ? 0 : image.height();

    ImageModel model;
    model.setFilename(fileInfo.fileName());
    model.setMimeType(mimeType);
    model.setWidth(width);
    model.setHeight(height);
    model.setTags(tags);

    return create(model, imageData);
}

QString ImageRepository::uploadFromQImage(const QString& filename, const QImage& image,
                                          const QString& format, const QStringList& tags)
{
    if (image.isNull()) {
        qWarning() << "Cannot upload null QImage";
        return QString();
    }

    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);

    if (!image.save(&buffer, format.toLatin1().constData())) {
        qWarning() << "Failed to convert QImage to format:" << format;
        return QString();
    }
    buffer.close();

    QString mimeType = "image/" + format.toLower();
    if (format.toUpper() == "JPG") {
        mimeType = "image/jpeg";
    }

    ImageModel model;
    model.setFilename(filename);
    model.setMimeType(mimeType);
    model.setWidth(image.width());
    model.setHeight(image.height());
    model.setTags(tags);

    return create(model, imageData);
}

// ============ Redis key 生成 ============

QString ImageRepository::keyImageData(const QString& id) const
{
    return QString("img:data:%1").arg(id);
}

QString ImageRepository::keyImageMeta(const QString& id) const
{
    return QString("img:meta:%1").arg(id);
}

QString ImageRepository::keyTagIndex(const QString& tag) const
{
    return QString("img:tag:%1").arg(tag);
}

QString ImageRepository::keyAllIds() const
{
    return "img:all";
}

QString ImageRepository::keyStats() const
{
    return "img:stats";
}

// ============ 内部辅助方法 ============

bool ImageRepository::saveMetadata(const QString& id, const ImageModel& model)
{
    QString metaKey = keyImageMeta(id);
    QVariantMap variantMap = model.toVariantMap();

    for (auto it = variantMap.begin(); it != variantMap.end(); ++it) {
        if (!m_redis.hSet(metaKey, it.key(), it.value().toString())) {
            qWarning() << "Failed to set field" << it.key() << "for metadata:" << id;
            return false;
        }
    }

    return true;
}

bool ImageRepository::updateTagIndex(const QString& id, const QStringList& oldTags, const QStringList& newTags)
{
    // 从旧标签集合中移除
    for (const QString& tag : oldTags) {
        if (!newTags.contains(tag)) {
            m_redis.sRem(keyTagIndex(tag), id);
        }
    }

    // 添加到新标签集合
    for (const QString& tag : newTags) {
        if (!oldTags.contains(tag)) {
            m_redis.sAdd(keyTagIndex(tag), id);
        }
    }

    return true;
}

bool ImageRepository::addToGlobalSet(const QString& id)
{
    return m_redis.sAdd(keyAllIds(), id);
}

bool ImageRepository::removeFromGlobalSet(const QString& id)
{
    return m_redis.sRem(keyAllIds(), id);
}

bool ImageRepository::updateStatistics(int countChange, qint64 sizeChange)
{
    QString statsKey = keyStats();
    
    int currentCount = count();
    int newCount = qMax(0, currentCount + countChange);
    m_redis.hSet(statsKey, "total_count", QString::number(newCount));

    qint64 currentSize = getTotalSize();
    qint64 newSize = qMax(0LL, currentSize + sizeChange);
    m_redis.hSet(statsKey, "total_size", QString::number(newSize));

    return true;
}

// ============ 工具方法 ============

QString ImageRepository::generateId()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString ImageRepository::getMimeTypeFromExtension(const QString& extension)
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
