#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <QString>
#include <QByteArray>
#include <QImage>
#include <QStringList>
#include "image_model.h"

class RedisManager;

/**
 * @brief 图片管理器类
 *
 * 提供图片的上传、检索、删除、搜索等功能
 */
class ImageManager
{
public:
    explicit ImageManager(RedisManager& redisManager);
    ~ImageManager();

    /**
    * @brief 上传图片
    *
    * 从文件,qimage,文件路径上传
    */
    QString uploadImage(const QString& filename,
                       const QByteArray& imageData,
                       const QString& mimeType,
                       const QStringList& tags = QStringList());
    QString uploadImageFromQImage(const QString& filename,
                                  const QImage& image,
                                  const QString& format = "PNG",
                                  const QStringList& tags = QStringList());
    QString uploadImageFromFile(const QString& filePath,
                                const QStringList& tags = QStringList());

    /**
    * @brief 检索图片
    *
    * 获取图片数据,获取qimage类型图片数据,获取图片元数据,保存图片到文件
    */
    QByteArray getImageData(const QString& id);
    QImage getImageAsQImage(const QString& id);
    ImageModel getImageMetadata(const QString& id);
    bool saveImageToFile(const QString& id, const QString& filepath);

    /**
    * @brief 元数据管理
    *
    * 更新元数据,更新tags,添加tag到图片,删除图片标签
    */
    bool updateMetadata(const QString& id, const ImageModel& metadata);
    bool updateTags(const QString& id, const QStringList& tags);
    bool addTagToImage(const QString& id, const QString& tag);
    bool removeTagFromImage(const QString& id, const QString& tag);

    /**
    * @brief 搜索功能
    *
    * 查询所有图片的id, 通过tag搜索图片id,通过多tag搜索图片id(and),通过多tag搜索图片id(or)
    */
    QStringList getAllImageIds();
    QStringList searchByTag(const QString& tag);
    QStringList searchByAllTags(const QStringList& tags);
    QStringList searchByAnyTag(const QStringList& tags);
    QStringList getImageIdsPaginated(int offset, int limit);

    /**
    * @brief 删除功能
    *
    * 删除图片,删除多图片,删除所有图片
    */
    bool deleteImage(const QString& id);
    int deleteImages(const QStringList& ids);
    bool clearAllImages();

    /**
    * @brief 统计功能
    *
    * 获取图片数量,获取图片总体积,获取图片状态
    */
    int getImageCount();
    qint64 getTotalSize();
    QString getStatistics();

    /**
    * @brief 工具
    *
    * 查询图片是否存在,生成图片id,从文件路径获取mime类型,通过扩展名获取mime类型
    */
    bool imageExists(const QString& id);
    static QString generateImageId();
    static QString getMimeTypeFromFilePath(const QString& filePath);
    static QString getMimeTypeFromExtension(const QString& extension);

private:
    RedisManager& m_redisManager;

    /**
    * @brief Redis key helpers
    */
    QString getImageDataKey(const QString& id) const;
    QString getMetadataKey(const QString& id) const;
    QString getTagSetKey(const QString& tag) const;
    QString getAllIdsKey() const;
    QString getStatsKey() const;

    /**
    * @brief Internal operations
    */
    bool saveMetadata(const QString& id, const ImageModel& metadata);
    bool updateTagIndex(const QString& id, const QStringList& oldTags, const QStringList& newTags);
    bool addToGlobalSet(const QString& id);
    bool removeFromGlobalSet(const QString& id);
    bool updateStatistics(int countChange, qint64 sizeChange);
};

#endif // IMAGE_MANAGER_H
