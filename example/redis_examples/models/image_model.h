#ifndef IMAGE_MODEL_H
#define IMAGE_MODEL_H

#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QVariantMap>
#include <memory>

/**
 * @brief 图片模型类
 *
 * 第2层：Model 层 - 纯数据结构，不含 Redis 逻辑
 * 负责：定义数据结构、验证规则、序列化/反序列化
 */
class ImageModel
{
public:
    ImageModel();
    ImageModel(const QString& id, const QString& filename,
               const QString& mimeType, qint64 size,
               int width = 0, int height = 0);
    ImageModel(const ImageModel& other);
    ImageModel& operator=(const ImageModel& other);
    ~ImageModel();

    // ============ 数据结构体 ============
    struct Data {
        QString id;
        QString filename;
        QString mimeType;
        qint64 size;
        QDateTime uploadTime;
        int width;
        int height;
        QStringList tags;

        // Redis Hash field names
        static const QString FIELD_ID;
        static const QString FIELD_FILENAME;
        static const QString FIELD_MIME_TYPE;
        static const QString FIELD_SIZE;
        static const QString FIELD_UPLOAD_TIME;
        static const QString FIELD_WIDTH;
        static const QString FIELD_HEIGHT;
        static const QString FIELD_TAGS;

        Data();
        static Data fromVariantMap(const QVariantMap& map);
        QVariantMap toVariantMap() const;
    };

    // ============ Getters ============
    QString getId() const { return d->id; }
    QString getFilename() const { return d->filename; }
    QString getMimeType() const { return d->mimeType; }
    qint64 getSize() const { return d->size; }
    QDateTime getUploadTime() const { return d->uploadTime; }
    int getWidth() const { return d->width; }
    int getHeight() const { return d->height; }
    QStringList getTags() const { return d->tags; }

    // ============ Setters（带验证）============
    bool setId(const QString& id);
    bool setFilename(const QString& filename);
    bool setMimeType(const QString& mimeType);
    bool setSize(qint64 size);
    bool setUploadTime(const QDateTime& time);
    bool setWidth(int width);
    bool setHeight(int height);
    bool setTags(const QStringList& tags);

    // ============ Tag 管理 ============
    bool addTag(const QString& tag);
    bool removeTag(const QString& tag);
    bool hasTag(const QString& tag) const;
    void clearTags();

    // ============ 验证方法 ============
    bool isValid() const;

    // ============ 工具方法 ============
    QString toString() const;
    QString getSizeFormatted() const;
    QString getDimensionsString() const;

    // ============ 序列化方法 ============
    static ImageModel fromVariantMap(const QVariantMap& map);
    QVariantMap toVariantMap() const;

    // ============ Redis 字段名访问器 ============
    static QString FIELD_ID() { return Data::FIELD_ID; }
    static QString FIELD_FILENAME() { return Data::FIELD_FILENAME; }
    static QString FIELD_MIME_TYPE() { return Data::FIELD_MIME_TYPE; }
    static QString FIELD_SIZE() { return Data::FIELD_SIZE; }
    static QString FIELD_UPLOAD_TIME() { return Data::FIELD_UPLOAD_TIME; }
    static QString FIELD_WIDTH() { return Data::FIELD_WIDTH; }
    static QString FIELD_HEIGHT() { return Data::FIELD_HEIGHT; }
    static QString FIELD_TAGS() { return Data::FIELD_TAGS; }

    // ============ 比较运算符 ============
    bool operator==(const ImageModel& other) const;
    bool operator!=(const ImageModel& other) const;

private:
    std::shared_ptr<Data> d;
};

#endif // IMAGE_MODEL_H
