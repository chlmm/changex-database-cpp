#ifndef IMAGE_MANAGEMENT_DEMO_H
#define IMAGE_MANAGEMENT_DEMO_H

#include <QString>

class ImageManager;

/**
 * @brief 完整的图片管理示例
 *
 * 演示完整的图片管理流程，包括上传、检索、更新、搜索和删除
 */
class ImageManagementDemo
{
public:
    static QString name() { return "Image Management Demo"; }
    static QString description() { return "Complete image management workflow: upload, retrieve, update, search, and delete images"; }

    static void run(ImageManager& imageManager);
};

#endif // IMAGE_MANAGEMENT_DEMO_H
