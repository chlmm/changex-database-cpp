#include "image_model.h"
#include <QDebug>

// ============ Data 结构体的静态成员初始化 ============
const QString ImageModel::Data::FIELD_ID = "id";
const QString ImageModel::Data::FIELD_FILENAME = "filename";
const QString ImageModel::Data::FIELD_MIME_TYPE = "mime_type";
const QString ImageModel::Data::FIELD_SIZE = "size";
const QString ImageModel::Data::FIELD_UPLOAD_TIME = "upload_time";
const QString ImageModel::Data::FIELD_WIDTH = "width";
const QString ImageModel::Data::FIELD_HEIGHT = "height";
const QString ImageModel::Data::FIELD_TAGS = "tags";

// ============ Data 结构体实现 ============
ImageModel::Data::Data()
    : size(0)
    , uploadTime(QDateTime::currentDateTime())
    , width(0)
    , height(0)
{
}

ImageModel::Data ImageModel::Data::fromVariantMap(const QVariantMap& map)
{
    Data data;

    if (map.contains(FIELD_ID)) {
        data.id = map.value(FIELD_ID).toString();
    }

    if (map.contains(FIELD_FILENAME)) {
        data.filename = map.value(FIELD_FILENAME).toString();
    }

    if (map.contains(FIELD_MIME_TYPE)) {
        data.mimeType = map.value(FIELD_MIME_TYPE).toString();
    }

    if (map.contains(FIELD_SIZE)) {
        data.size = map.value(FIELD_SIZE).toLongLong();
    }

    if (map.contains(FIELD_WIDTH)) {
        data.width = map.value(FIELD_WIDTH).toInt();
    }

    if (map.contains(FIELD_HEIGHT)) {
        data.height = map.value(FIELD_HEIGHT).toInt();
    }

    if (map.contains(FIELD_UPLOAD_TIME)) {
        data.uploadTime = QDateTime::fromMSecsSinceEpoch(
            map.value(FIELD_UPLOAD_TIME).toULongLong()
        );
    }

    if (map.contains(FIELD_TAGS)) {
        QString tagsStr = map.value(FIELD_TAGS).toString();
        data.tags = tagsStr.split(",", Qt::SkipEmptyParts);
    }

    return data;
}

QVariantMap ImageModel::Data::toVariantMap() const
{
    QVariantMap map;
    map[FIELD_ID] = id;
    map[FIELD_FILENAME] = filename;
    map[FIELD_MIME_TYPE] = mimeType;
    map[FIELD_SIZE] = size;
    map[FIELD_WIDTH] = width;
    map[FIELD_HEIGHT] = height;
    map[FIELD_UPLOAD_TIME] = uploadTime.toMSecsSinceEpoch();
    map[FIELD_TAGS] = tags.join(",");

    return map;
}

// ============ ImageModel 构造函数和析构函数 ============
ImageModel::ImageModel()
    : d(std::make_shared<Data>())
{
}

ImageModel::ImageModel(const QString& id, const QString& filename,
                       const QString& mimeType, qint64 size,
                       int width, int height)
    : d(std::make_shared<Data>())
{
    d->id = id;
    d->filename = filename;
    d->mimeType = mimeType;
    d->size = size;
    d->width = width;
    d->height = height;
    d->uploadTime = QDateTime::currentDateTime();
}

ImageModel::ImageModel(const ImageModel& other)
    : d(other.d)
{
    // shared_ptr 的拷贝构造会自动处理引用计数
}

ImageModel& ImageModel::operator=(const ImageModel& other)
{
    if (this != &other) {
        d = other.d;
    }
    return *this;
}

ImageModel::~ImageModel()
{
    // shared_ptr 会自动释放内存
}

// ============ Setters（带验证）============
bool ImageModel::setId(const QString& id)
{
    if (id.isEmpty()) {
        qWarning() << "ID cannot be empty";
        return false;
    }
    // 创建新的 Data 对象（写时拷贝）
    d = std::make_shared<Data>(*d);
    d->id = id;
    return true;
}

bool ImageModel::setFilename(const QString& filename)
{
    if (filename.isEmpty()) {
        qWarning() << "Filename cannot be empty";
        return false;
    }
    d = std::make_shared<Data>(*d);
    d->filename = filename;
    return true;
}

bool ImageModel::setMimeType(const QString& mimeType)
{
    if (mimeType.isEmpty()) {
        qWarning() << "MIME type cannot be empty";
        return false;
    }
    d = std::make_shared<Data>(*d);
    d->mimeType = mimeType;
    return true;
}

bool ImageModel::setSize(qint64 size)
{
    if (size < 0) {
        qWarning() << "Size cannot be negative";
        return false;
    }
    d = std::make_shared<Data>(*d);
    d->size = size;
    return true;
}

bool ImageModel::setUploadTime(const QDateTime& time)
{
    if (!time.isValid()) {
        qWarning() << "Invalid upload time";
        return false;
    }
    d = std::make_shared<Data>(*d);
    d->uploadTime = time;
    return true;
}

bool ImageModel::setWidth(int width)
{
    if (width < 0) {
        qWarning() << "Width cannot be negative";
        return false;
    }
    d = std::make_shared<Data>(*d);
    d->width = width;
    return true;
}

bool ImageModel::setHeight(int height)
{
    if (height < 0) {
        qWarning() << "Height cannot be negative";
        return false;
    }
    d = std::make_shared<Data>(*d);
    d->height = height;
    return true;
}

bool ImageModel::setTags(const QStringList& tags)
{
    d = std::make_shared<Data>(*d);
    d->tags = tags;
    return true;
}

// ============ Tag 管理 ============
bool ImageModel::addTag(const QString& tag)
{
    if (tag.isEmpty()) {
        qWarning() << "Cannot add empty tag";
        return false;
    }

    if (d->tags.contains(tag)) {
        return true; // 标签已存在，不重复添加
    }

    d = std::make_shared<Data>(*d);
    d->tags.append(tag);
    return true;
}

bool ImageModel::removeTag(const QString& tag)
{
    if (!d->tags.contains(tag)) {
        return false; // 标签不存在
    }

    d = std::make_shared<Data>(*d);
    d->tags.removeAll(tag);
    return true;
}

bool ImageModel::hasTag(const QString& tag) const
{
    return d->tags.contains(tag);
}

void ImageModel::clearTags()
{
    if (d->tags.isEmpty()) {
        return;
    }
    d = std::make_shared<Data>(*d);
    d->tags.clear();
}

// ============ 验证方法 ============
bool ImageModel::isValid() const
{
    return !d->id.isEmpty() &&
           !d->filename.isEmpty() &&
           !d->mimeType.isEmpty() &&
           d->size >= 0 &&
           d->width >= 0 &&
           d->height >= 0 &&
           d->uploadTime.isValid();
}

// ============ 工具方法 ============
QString ImageModel::toString() const
{
    return QString("ImageModel[id=%1, filename=%2, type=%3, size=%4, dimensions=%5, tags=%6]")
           .arg(d->id)
           .arg(d->filename)
           .arg(d->mimeType)
           .arg(getSizeFormatted())
           .arg(getDimensionsString())
           .arg(d->tags.join(", "));
}

QString ImageModel::getSizeFormatted() const
{
    if (d->size < 1024) {
        return QString("%1 B").arg(d->size);
    } else if (d->size < 1024 * 1024) {
        return QString("%1 KB").arg(d->size / 1024.0, 0, 'f', 2);
    } else if (d->size < 1024 * 1024 * 1024) {
        return QString("%1 MB").arg(d->size / (1024.0 * 1024.0), 0, 'f', 2);
    } else {
        return QString("%1 GB").arg(d->size / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
    }
}

QString ImageModel::getDimensionsString() const
{
    return QString("%1x%2").arg(d->width).arg(d->height);
}

// ============ 序列化方法 ============
ImageModel ImageModel::fromVariantMap(const QVariantMap& map)
{
    ImageModel model;
    model.d = std::make_shared<Data>(Data::fromVariantMap(map));
    return model;
}

QVariantMap ImageModel::toVariantMap() const
{
    return d->toVariantMap();
}

// ============ 比较运算符 ============
bool ImageModel::operator==(const ImageModel& other) const
{
    if (d == other.d) {
        return true; // 同一个 shared_ptr
    }

    return d->id == other.d->id &&
           d->filename == other.d->filename &&
           d->mimeType == other.d->mimeType &&
           d->size == other.d->size &&
           d->width == other.d->width &&
           d->height == other.d->height &&
           d->uploadTime == other.d->uploadTime &&
           d->tags == other.d->tags;
}

bool ImageModel::operator!=(const ImageModel& other) const
{
    return !(*this == other);
}
