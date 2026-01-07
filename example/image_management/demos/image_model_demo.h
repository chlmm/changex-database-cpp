#ifndef IMAGE_MODEL_DEMO_H
#define IMAGE_MODEL_DEMO_H

#include <QString>

class ImageManager;

/**
 * @brief 图片元数据管理示例
 *
 * 演示如何使用ImageManager管理图片元数据
 */
class ImageModelDemo
{
public:
    static QString name() { return "Image Metadata Demo"; }
    static QString description() { return "Demonstrate image metadata management: update metadata, add/remove tags, modify tags"; }

    static void run(ImageManager& imageManager);
};

#endif // IMAGE_MODEL_DEMO_H
