#ifndef IMAGE_SEARCH_DEMO_H
#define IMAGE_SEARCH_DEMO_H

#include <QString>

class ImageManager;

/**
 * @brief 图片搜索示例
 *
 * 演示如何使用ImageManager搜索图片
 */
class ImageSearchDemo
{
public:
    static QString name() { return "Image Search Demo"; }
    static QString description() { return "Demonstrate image search functionality: search by tag, by multiple tags, and pagination"; }

    static void run(ImageManager& imageManager);
};

#endif // IMAGE_SEARCH_DEMO_H
