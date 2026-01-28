#ifndef IMAGE_REPOSITORY_H
#define IMAGE_REPOSITORY_H

#include <QString>
#include <QByteArray>
#include <QImage>
#include <QStringList>
#include <QList>
#include "../models/image_model.h"

class RedisManager;

/**
 * @brief 图片仓库类
 *
 * 第3层：Repository 层 - 唯一与 Redis 交互的地方
 * 负责：封装所有 Redis 数据访问操作（CRUD）
 */
class ImageRepository
{
public:
    explicit ImageRepository(RedisManager& redisManager);
    ~ImageRepository();

    // ============ 增删改查 ============
    
    /**
     * @brief 创建图片（上传）
     * @param model 图片元数据
     * @param imageData 图片二进制数据
     * @return 图片ID，失败返回空字符串
     */
    QString create(const ImageModel& model, const QByteArray& imageData);
    
    /**
     * @brief 根据ID查找图片元数据
     * @param id 图片ID
     * @return ImageModel，不存在时返回空模型
     */
    ImageModel findById(const QString& id);
    
    /**
     * @brief 获取图片二进制数据
     * @param id 图片ID
     * @return 图片数据，失败返回空 QByteArray
     */
    QByteArray getImageData(const QString& id);
    
    /**
     * @brief 获取图片作为 QImage
     * @param id 图片ID
     * @return QImage，失败返回空图像
     */
    QImage getImageAsQImage(const QString& id);
    
    /**
     * @brief 更新图片元数据
     * @param model 新的图片元数据（必须包含有效ID）
     * @return 是否成功
     */
    bool update(const ImageModel& model);
    
    /**
     * @brief 删除图片
     * @param id 图片ID
     * @return 是否成功
     */
    bool remove(const QString& id);
    
    /**
     * @brief 批量删除图片
     * @param ids 图片ID列表
     * @return 成功删除的数量
     */
    int removeBatch(const QStringList& ids);

    // ============ 查询 ============
    
    /**
     * @brief 获取所有图片
     * @return 图片模型列表
     */
    QList<ImageModel> findAll();
    
    /**
     * @brief 分页获取图片ID
     * @param offset 起始位置
     * @param limit 数量限制
     * @return 图片ID列表
     */
    QStringList findIdsPaginated(int offset, int limit);
    
    /**
     * @brief 根据标签搜索图片
     * @param tag 标签
     * @return 符合条件的图片模型列表
     */
    QList<ImageModel> searchByTag(const QString& tag);
    
    /**
     * @brief 根据多个标签搜索图片
     * @param tags 标签列表
     * @param matchAll true=必须包含所有标签(AND), false=包含任一标签(OR)
     * @return 符合条件的图片模型列表
     */
    QList<ImageModel> searchByTags(const QStringList& tags, bool matchAll = true);

    // ============ Tag 管理 ============
    
    /**
     * @brief 为图片添加标签
     * @param id 图片ID
     * @param tag 标签
     * @return 是否成功
     */
    bool addTag(const QString& id, const QString& tag);
    
    /**
     * @brief 为图片移除标签
     * @param id 图片ID
     * @param tag 标签
     * @return 是否成功
     */
    bool removeTag(const QString& id, const QString& tag);
    
    /**
     * @brief 更新图片的所有标签
     * @param id 图片ID
     * @param tags 新标签列表
     * @return 是否成功
     */
    bool updateTags(const QString& id, const QStringList& tags);

    // ============ 统计与工具 ============
    
    /**
     * @brief 获取图片总数
     * @return 图片数量
     */
    int count();
    
    /**
     * @brief 获取总存储大小
     * @return 总字节数
     */
    qint64 getTotalSize();
    
    /**
     * @brief 获取统计信息字符串
     * @return 格式化的统计信息
     */
    QString getStatistics();
    
    /**
     * @brief 检查图片是否存在
     * @param id 图片ID
     * @return 是否存在
     */
    bool exists(const QString& id);
    
    /**
     * @brief 清空所有图片数据
     * @return 是否成功
     */
    bool clearAll();
    
    /**
     * @brief 保存图片到文件
     * @param id 图片ID
     * @param filePath 文件路径
     * @return 是否成功
     */
    bool saveToFile(const QString& id, const QString& filePath);

    // ============ 便捷上传方法 ============
    
    /**
     * @brief 从文件上传图片
     * @param filePath 文件路径
     * @param tags 标签列表
     * @return 图片ID，失败返回空字符串
     */
    QString uploadFromFile(const QString& filePath, const QStringList& tags = QStringList());
    
    /**
     * @brief 从 QImage 上传图片
     * @param filename 文件名
     * @param image QImage 对象
     * @param format 格式 (PNG, JPEG 等)
     * @param tags 标签列表
     * @return 图片ID，失败返回空字符串
     */
    QString uploadFromQImage(const QString& filename, const QImage& image, 
                             const QString& format = "PNG",
                             const QStringList& tags = QStringList());

private:
    RedisManager& m_redis;

    // Redis key 生成
    QString keyImageData(const QString& id) const;
    QString keyImageMeta(const QString& id) const;
    QString keyTagIndex(const QString& tag) const;
    QString keyAllIds() const;
    QString keyStats() const;

    // 内部辅助方法
    bool saveMetadata(const QString& id, const ImageModel& model);
    bool updateTagIndex(const QString& id, const QStringList& oldTags, const QStringList& newTags);
    bool addToGlobalSet(const QString& id);
    bool removeFromGlobalSet(const QString& id);
    bool updateStatistics(int countChange, qint64 sizeChange);
    
    // 工具方法
    static QString generateId();
    static QString getMimeTypeFromExtension(const QString& extension);
};

#endif // IMAGE_REPOSITORY_H
