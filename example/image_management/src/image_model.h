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
 * 用于表示完整的图片数据模型，包括基本信息和元数据
 * 使用 Pimpl 模式实现高效的数据管理和清晰的接口
 */
class ImageModel
{
public:
    ImageModel();

    // 构造函数
    ImageModel(const QString& id, const QString& filename,
               const QString& mimeType, qint64 size,
               int width = 0, int height = 0);

    // 拷贝构造函数
    ImageModel(const ImageModel& other);

    // 赋值运算符
    ImageModel& operator=(const ImageModel& other);

    // 析构函数
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

        // 构造函数
        Data();

        // 从 QVariantMap 构造（用于 Redis 反序列化）
        static Data fromVariantMap(const QVariantMap& map);

        // 转换为 QVariantMap（用于 Redis 序列化）
        QVariantMap toVariantMap() const;
    };

    /**
    * @brief 获取元数据
    *
    * 获取图片id,文件名,mimetype,大小,上传时间,宽高,标签
    */
    QString getId() const { return d->id; }
    QString getFilename() const { return d->filename; }
    QString getMimeType() const { return d->mimeType; }
    qint64 getSize() const { return d->size; }
    QDateTime getUploadTime() const { return d->uploadTime; }
    int getWidth() const { return d->width; }
    int getHeight() const { return d->height; }
    QStringList getTags() const { return d->tags; }

    /**
    * @brief 设置元数据,带验证
    *
    * 设置图片id,文件名,mimetype,大小,上传时间,宽高,标签
    */
    bool setId(const QString& id);
    bool setFilename(const QString& filename);
    bool setMimeType(const QString& mimeType);
    bool setSize(qint64 size);
    bool setUploadTime(const QDateTime& time);
    bool setWidth(int width);
    bool setHeight(int height);
    bool setTags(const QStringList& tags);

    /**
    * @brief Tag 管理
    *
    * 添加,删除tag,查询是否存在某tag,清除所有tag
    */
    bool addTag(const QString& tag);
    bool removeTag(const QString& tag);
    bool hasTag(const QString& tag) const;
    void clearTags();

    /**
    * @brief 验证方法
    *
    * 检验xxx
    */
    bool isValid() const;

    // ============ 工具方法 ============
    QString toString() const;
    QString getSizeFormatted() const;

    /**
     * @brief 获取图像尺寸字符串
     * @return 尺寸字符串（如 "1920x1080"）
     */
    QString getDimensionsString() const;

    // ============ 序列化方法 ============
    /**
     * @brief 从 QVariantMap 创建模型（用于从 Redis 加载）
     * @param map QVariantMap 数据
     * @return 创建的 ImageModel
     */
    static ImageModel fromVariantMap(const QVariantMap& map);

    /**
     * @brief 将模型转换为 QVariantMap（用于保存到 Redis）
     * @return QVariantMap 数据
     */
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
    // Pimpl 模式：使用 shared_ptr 实现高效的拷贝语义
    std::shared_ptr<Data> d;
};

#endif // IMAGE_MODEL_H
