#ifndef IMAGE_RETRIEVE_DEMO_H
#define IMAGE_RETRIEVE_DEMO_H

#include <QString>

class ImageManager;

/**
 * @brief 图片检索示例
 *
 * 演示如何使用ImageManager检索图片
 */
class ImageRetrieveDemo
{
public:
    static QString name() { return "Image Retrieve Demo"; }
    static QString description() { return "Demonstrate image retrieval functionality: get image data, QImage objects, and save to file"; }

    static void run(ImageManager& imageManager);
};

#endif // IMAGE_RETRIEVE_DEMO_H
