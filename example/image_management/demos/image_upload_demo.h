#ifndef IMAGE_UPLOAD_DEMO_H
#define IMAGE_UPLOAD_DEMO_H

#include <QString>

class ImageManager;

/**
 * @brief 图片上传示例
 *
 * 演示如何使用ImageManager上传图片
 */
class ImageUploadDemo
{
public:
    static QString name() { return "Image Upload Demo"; }
    static QString description() { return "Demonstrate image upload functionality: upload images from files, QImage objects, and raw data"; }

    static void run(ImageManager& imageManager);
};

#endif // IMAGE_UPLOAD_DEMO_H
