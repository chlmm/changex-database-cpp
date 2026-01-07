# Image Management Example Project

这是一个使用 Qt Redis Plus Plus Extended 库实现的完整图片管理系统示例，演示如何使用 Redis 存储和管理图片数据。

## 功能特性

### 核心功能

1. **图片存储**
   - 从文件上传图片
   - 从 QImage 对象上传图片
   - 从原始二进制数据上传图片
   - 支持多种图片格式（JPG、PNG、GIF、BMP等）

2. **图片管理**
   - 存储图片元数据（文件名、大小、类型、尺寸、上传时间等）
   - 标签管理（添加、删除、更新标签）
   - 生成唯一图片ID（基于UUID）

3. **图片检索**
   - 通过ID获取图片数据
   - 将图片数据转换为QImage对象
   - 获取图片元数据
   - 批量获取图片列表
   - 分页查询

4. **图片搜索**
   - 按单个标签搜索
   - 按多个标签搜索（AND操作 - 必须包含所有标签）
   - 按多个标签搜索（OR操作 - 包含任意标签）
   - 按标签索引快速检索

5. **图片删除**
   - 删除单个图片
   - 批量删除图片
   - 清空所有图片

6. **统计功能**
   - 获取图片总数
   - 获取总存储大小
   - 获取详细统计信息

## 技术设计

### Redis 数据结构

项目使用以下 Redis 数据结构：

1. **图片数据存储** (String)
   - Key: `image:<id>`
   - Value: 图片二进制数据

2. **图片元数据** (Hash)
   - Key: `image:meta:<id>`
   - Fields:
     - `id`: 图片ID
     - `filename`: 文件名
     - `mime_type`: MIME类型（如 image/jpeg）
     - `size`: 文件大小（字节）
     - `width`: 图片宽度
     - `height`: 图片高度
     - `upload_time`: 上传时间戳
     - `tags`: 标签（逗号分隔）

3. **标签索引** (Set)
   - Key: `image:tags:<tag>`
   - Members: 包含该标签的图片ID集合

4. **所有图片列表** (Set)
   - Key: `image:all_ids`
   - Members: 所有图片ID

5. **统计信息** (Hash)
   - Key: `image:stats`
   - Fields:
     - `total_count`: 图片总数
     - `total_size`: 总大小（字节）

### 项目结构

```
image_management/
├── CMakeLists.txt                    # 构建配置文件
├── main.cpp                          # 主程序入口
├── README.md                         # 项目文档
├── src/                              # 核心源代码
│   ├── image_manager.h/.cpp         # 图片管理器核心类
│   └── image_metadata.h/.cpp        # 图片元数据类
└── demos/                            # 示例代码
    ├── image_upload_demo.h/.cpp     # 图片上传示例
    ├── image_retrieve_demo.h/.cpp   # 图片检索示例
    ├── image_metadata_demo.h/.cpp   # 元数据管理示例
    ├── image_search_demo.h/.cpp     # 图片搜索示例
    └── image_management_demo.h/.cpp # 完整管理流程示例
```

## 前置要求

- Qt 5.12 或更高版本
- Redis 服务器运行在 `127.0.0.1:6379`
- redis-plus-plus 库
- hiredis 库
- RedisModule 库

## 构建

在项目根目录执行：

```bash
mkdir build
cd build
cmake ..
make
```

编译后的可执行文件位于 `build/image_management_example`。

## 使用方法

### 交互式模式

不带参数运行示例程序，进入交互式菜单模式：

```bash
./image_management_example
```

你将看到包含可用示例的菜单：

```
Qt Redis Plus Plus Extended - Image Management Demo
==================================================
Available Demos:
1. Image Upload Demo - 图片上传
2. Image Retrieve Demo - Retrieve image data, QImage objects, and save to file
3. Image Metadata Demo - Manage image metadata and tags
4. Image Search Demo - Search images by tags with pagination
5. Complete Management Demo - Full workflow: upload, retrieve, update, search, delete
6. Run All Demos
0. Exit

Select demo to run (0-6):
```

### 命令行模式

你也可以直接使用命令行参数运行特定示例：

```bash
# 通过名称或编号运行特定示例
./image_management_example upload        # 或 ./image_management_example 1
./image_management_example retrieve      # 或 ./image_management_example 2
./image_management_example metadata      # 或 ./image_management_example 3
./image_management_example search        # 或 ./image_management_example 4
./image_management_example complete      # 或 ./image_management_example 5

# 运行所有示例
./image_management_example all           # 或 ./image_management_example 6

# 显示帮助信息
./image_management_example --help
```

## 示例说明

### 1. Image Upload Demo

演示图片上传功能：
- 从 QImage 对象上传不同类型的图片（红色方块、渐变图、棋盘格等）
- 从原始字节数组上传图片
- 为图片添加标签
- 显示上传后的元数据

**输出示例：**
```
=== Image Upload Demo ===
Demonstrate image upload functionality: upload images from files, QImage objects, and raw data

1. Creating and uploading a simple red square (100x100)...
   Success! Image ID: a1b2c3d4-e5f6-7890-abcd-ef1234567890
   Metadata: Image[id=a1b2c3d4-e5f6..., filename=red_square.png, type=image/png, size=318 B, tags=simple, red]
```

### 2. Image Retrieve Demo

演示图片检索功能：
- 获取图片二进制数据
- 将图片转换为 QImage 对象
- 获取图片元数据
- 保存图片到本地文件
- 分页获取图片列表
- 检查图片是否存在

**输出示例：**
```
=== Image Retrieve Demo ===
Demonstrate image retrieval functionality: get image data, QImage objects, and save to file

Found 4 images in the database.

1. Retrieving image data as QByteArray...
   Success! Retrieved 318 bytes
   Image: Image[id=a1b2c3d4-e5f6..., filename=red_square.png, type=image/png, size=318 B, tags=simple, red]
```

### 3. Image Metadata Demo

演示元数据管理功能：
- 获取图片元数据
- 添加标签
- 删除标签
- 批量更新标签
- 更新完整元数据
- 验证更新结果

**输出示例：**
```
=== Image Metadata Demo ===
Demonstrate image metadata management: update metadata, add/remove tags, modify tags

Using image ID: a1b2c3d4-e5f6...

1. Retrieving current metadata...
   Current metadata: Image[id=a1b2c3d4-e5f6..., filename=red_square.png, type=image/png, size=318 B, tags=simple, red]
   Tags: simple, red
```

### 4. Image Search Demo

演示图片搜索功能：
- 获取所有图片ID
- 按单个标签搜索
- 按多个标签搜索（AND操作）
- 按多个标签搜索（OR操作）
- 分页查询
- 显示详细搜索结果

**输出示例：**
```
=== Image Search Demo ===
Demonstrate image search functionality: search by tag, by multiple tags, and pagination

Found 7 images in the database.

1. Retrieving all image IDs...
   Total images: 7
   First 5 IDs:
      1. a1b2c3d4-e5f6...
      2. b2c3d4e5-f6a7...
```

### 5. Complete Management Demo

演示完整的图片管理流程：
- 清空现有图片
- 上传多张带不同标签的图片
- 显示所有上传的图片
- 按标签搜索图片
- 更新图片元数据和标签
- 检索并保存图片到本地文件
- 多标签搜索
- 显示数据库统计信息
- 删除指定图片
- 显示最终状态

**输出示例：**
```
=== Image Management Demo ===
Complete image management workflow: upload, retrieve, update, search, and delete images

Step 1: Clearing any existing images...
   Database cleared successfully.

Step 2: Uploading images...
   Uploaded 'landscape.jpg' with tags: landscape, nature
   Uploaded 'portrait.jpg' with tags: portrait, people
   Uploaded 'abstract.jpg' with tags: abstract, art

Step 3: Uploading additional images for testing...
   Total images uploaded: 5

Step 4: Displaying all uploaded images...
   Found 5 images:
```

## 核心类说明

### ImageMetadata

图片元数据类，用于存储图片的基本信息：

```cpp
class ImageMetadata {
public:
    QString getId() const;              // 获取图片ID
    QString getFilename() const;        // 获取文件名
    QString getMimeType() const;        // 获取MIME类型
    qint64 getSize() const;             // 获取文件大小
    QDateTime getUploadTime() const;    // 获取上传时间
    int getWidth() const;               // 获取图片宽度
    int getHeight() const;              // 获取图片高度
    QStringList getTags() const;        // 获取标签列表

    void addTag(const QString& tag);    // 添加标签
    void removeTag(const QString& tag); // 删除标签
    bool hasTag(const QString& tag) const; // 检查是否包含标签
    QString toString() const;           // 转换为字符串描述
};
```

## 设计特点

1. **模块化设计**
   - 每个示例都是独立的类，易于理解和扩展
   - 核心功能封装在 ImageManager 中
   - 清晰的接口和职责分离

2. **灵活的数据结构**
   - 使用 Redis String 存储图片二进制数据
   - 使用 Redis Hash 存储图片元数据
   - 使用 Redis Set 实现高效的标签索引和搜索

3. **完整的 CRUD 操作**
   - Create: 上传图片
   - Read: 检索图片和元数据
   - Update: 更新元数据和标签
   - Delete: 删除图片

4. **高效搜索**
   - 基于标签的快速索引
   - 支持多条件搜索（AND/OR）
   - 分页支持，适合大数据量

5. **错误处理**
   - 完善的错误检查
   - 清晰的错误日志
   - 用户友好的错误提示

## 应用场景

这个示例可以用于：

- **图片管理系统**: 网站或应用的图片存储和管理
- **内容管理系统**: 博客、新闻网站的图片管理
- **相册应用**: 个人或企业相册的图片存储
- **图床服务**: 云存储图床的实现参考
- **学习参考**: Redis 图片存储的最佳实践

## 故障排除

### 连接 Redis 服务器失败

确保 Redis 正在运行：

```bash
redis-server
```

### 编译错误

确保安装了所有依赖项：

- Qt 5.12+
- redis-plus-plus
- hiredis
- RedisModule

### 图片上传失败

检查图片格式是否支持，确保图片数据不为空。

## 扩展建议

可以考虑添加以下功能：

1. **图片压缩**: 在上传前压缩图片
2. **缩略图生成**: 自动生成并存储缩略图
3. **图片过期**: 支持设置图片过期时间
4. **访问控制**: 添加用户权限管理
5. **图片分类**: 支持多级分类系统
6. **批量操作**: 优化批量上传和删除性能
7. **备份恢复**: 实现数据备份和恢复功能

## 许可证

本示例项目遵循与 Qt Redis Plus Plus Extended 库相同的许可证。
