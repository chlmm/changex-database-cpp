#ifndef IMAGE_SCENARIOS_H
#define IMAGE_SCENARIOS_H

class ImageRepository;

/**
 * @brief 图片业务场景类
 *
 * 第4层：Scenario 层 - 面向用户的业务场景
 * 负责：组合 Repository 实现完整的业务流程
 */
class ImageScenarios
{
public:
    // ============ 上传场景 ============
    
    /**
     * @brief 场景1：从文件上传图片
     * @param repo 图片仓库
     */
    static void uploadFromFile(ImageRepository& repo);
    
    /**
     * @brief 场景2：创建示例图片并上传（无需外部文件）
     * @param repo 图片仓库
     */
    static void uploadSampleImage(ImageRepository& repo);
    
    /**
     * @brief 场景3：批量上传示例图片
     * @param repo 图片仓库
     */
    static void uploadBatchSamples(ImageRepository& repo);

    // ============ 检索场景 ============
    
    /**
     * @brief 场景4：列出所有图片
     * @param repo 图片仓库
     */
    static void listAllImages(ImageRepository& repo);
    
    /**
     * @brief 场景5：检索并显示图片详情
     * @param repo 图片仓库
     */
    static void retrieveAndDisplay(ImageRepository& repo);
    
    /**
     * @brief 场景6：保存图片到本地文件
     * @param repo 图片仓库
     */
    static void saveImageToFile(ImageRepository& repo);

    // ============ 元数据管理场景 ============
    
    /**
     * @brief 场景7：管理图片标签（添加/删除）
     * @param repo 图片仓库
     */
    static void manageTags(ImageRepository& repo);
    
    /**
     * @brief 场景8：更新图片元数据
     * @param repo 图片仓库
     */
    static void updateMetadata(ImageRepository& repo);

    // ============ 搜索场景 ============
    
    /**
     * @brief 场景9：按标签搜索图片
     * @param repo 图片仓库
     */
    static void searchByTag(ImageRepository& repo);
    
    /**
     * @brief 场景10：高级标签搜索（AND/OR）
     * @param repo 图片仓库
     */
    static void advancedTagSearch(ImageRepository& repo);

    // ============ 删除场景 ============
    
    /**
     * @brief 场景11：删除指定图片
     * @param repo 图片仓库
     */
    static void deleteImage(ImageRepository& repo);
    
    /**
     * @brief 场景12：批量删除图片
     * @param repo 图片仓库
     */
    static void deleteBatchImages(ImageRepository& repo);
    
    /**
     * @brief 场景13：清空所有图片
     * @param repo 图片仓库
     */
    static void clearAllImages(ImageRepository& repo);

    // ============ 高级场景 ============
    
    /**
     * @brief 场景14：显示图片为 QImage
     * @param repo 图片仓库
     */
    static void displayAsQImage(ImageRepository& repo);
    
    /**
     * @brief 场景15：分页浏览图片
     * @param repo 图片仓库
     */
    static void browseWithPagination(ImageRepository& repo);
    
    /**
     * @brief 场景16：检查图片存在性
     * @param repo 图片仓库
     */
    static void checkImageExistence(ImageRepository& repo);

    // ============ 统计场景 ============
    
    /**
     * @brief 场景13：显示统计信息
     * @param repo 图片仓库
     */
    static void showStatistics(ImageRepository& repo);

    // ============ 完整工作流场景 ============
    
    /**
     * @brief 场景14：完整工作流演示
     * 上传 → 检索 → 修改标签 → 搜索 → 删除
     * @param repo 图片仓库
     */
    static void completeWorkflow(ImageRepository& repo);
};

#endif // IMAGE_SCENARIOS_H
