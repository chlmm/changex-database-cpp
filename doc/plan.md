# RedisModule 架构改进计划

## 背景

当前 RedisModule 是一个 Qt 绑定的 Redis 客户端库，接口使用 Qt 类型（QString、QByteArray 等）。这限制了库的可重用性和可测试性。本计划旨在将其重构为**标准 C++ 核心 + Qt 适配器**的架构。

---

## 目标

1. **框架解耦**：核心库仅依赖标准 C++17，不依赖 Qt
2. **可重用性**：核心库可用于 CLI、后端服务、其他 GUI 框架
3. **可测试性**：单元测试无需 Qt 环境
4. **向后兼容**：现有 Qt 项目可无缝迁移
5. **可选编译**：通过 CMake 选项控制是否编译 Qt 适配器

---

## 当前问题

### 隐式转换已存在
当前代码在每个操作类中都进行了 Qt 类型到 std::string 的转换：

```cpp
// redisstringoperations.cpp
connection_->redis()->set(
    key.toStdString(),              // QString → std::string
    value.toStdString()             // QString → std::string
);

// redisbytesoperations.cpp  
connection_->redis()->set(
    key.toStdString(),
    std::string(value.constData(), value.size())  // QByteArray → std::string
);
```

转换是不可避免的（因为 redis-plus-plus 只接受标准 C++ 类型）。改造只是将隐式转换变为显式适配。

---

## 改造方案

### 阶段一：目录结构调整

```
RedisModule/
├── core/                           # 纯 C++ 核心库（新增）
│   ├── CMakeLists.txt
│   ├── include/um/
│   │   ├── client.hpp              # 主客户端类（std::string 接口）
│   │   ├── types.hpp               # 类型定义（Bytes = std::vector<uint8_t>）
│   │   ├── connection.hpp          # Redis 连接管理
│   │   └── operations/             # 各数据类型操作
│   │       ├── string_operations.hpp
│   │       ├── bytes_operations.hpp
│   │       ├── hash_operations.hpp
│   │       ├── list_operations.hpp
│   │       ├── set_operations.hpp
│   │       ├── sorted_set_operations.hpp
│   │       ├── expiration_operations.hpp
│   │       └── transaction_operations.hpp
│   └── src/
│       ├── client.cpp
│       ├── connection.cpp
│       └── operations/
├── adapters/                       # 适配器层（新增）
│   └── qt/                         # Qt 适配器（可选编译）
│       ├── CMakeLists.txt
│       ├── include/um/qt/
│       │   └── qt_client.hpp       # Qt 客户端类（QString/QByteArray 接口）
│       └── src/
│           └── qt_client.cpp       # 类型转换实现
└── compat/                         # 向后兼容层（过渡）
    ├── redismanager.h              # 委托给 Qt 适配器
    └── redismanager.cpp
```

### 阶段二：CMake 配置改造

**根目录 CMakeLists.txt 修改：**

```cmake
# 新增选项
option(BUILD_QT_ADAPTER "Build Qt adapter module" ON)
option(BUILD_CORE_TESTS "Build core library tests" ON)
option(BUILD_QT_ADAPTER_TESTS "Build Qt adapter tests" ON)

# 核心库（必须）
add_subdirectory(RedisModule/core)

# Qt 适配器（可选）
if(BUILD_QT_ADAPTER)
    find_package(Qt5 REQUIRED COMPONENTS Core)
    add_subdirectory(RedisModule/adapters/qt)
endif()

# 向后兼容层
if(BUILD_QT_ADAPTER)
    add_subdirectory(RedisModule/compat)
endif()
```

### 阶段三：核心库接口设计

**core/include/um/client.hpp：**

```cpp
#pragma once
#include <string>
#include <vector>
#include <optional>
#include "types.hpp"
#include "operations/string_operations.hpp"
#include "operations/bytes_operations.hpp"
// ... 其他操作

namespace um {

using Bytes = std::vector<uint8_t>;
using OptionalString = std::optional<std::string>;
using OptionalBytes = std::optional<Bytes>;

class Client {
public:
    bool connect(const std::string& host, int port);
    void disconnect();
    bool isConnected() const;
    
    // String 操作
    bool set(const std::string& key, const std::string& value);
    OptionalString get(const std::string& key);
    
    // Bytes 操作（用于图片等二进制数据）
    bool setBytes(const std::string& key, const Bytes& value);
    OptionalBytes getBytes(const std::string& key);
    
    // Hash 操作
    bool hset(const std::string& key, const std::string& field, const std::string& value);
    OptionalString hget(const std::string& key, const std::string& field);
    
    // List 操作
    bool lpush(const std::string& key, const std::string& value);
    OptionalString lpop(const std::string& key);
    
    // Set 操作
    bool sadd(const std::string& key, const std::string& member);
    bool sismember(const std::string& key, const std::string& member);
    
    // Sorted Set 操作
    bool zadd(const std::string& key, double score, const std::string& member);
    
    // Expiration 操作
    bool expire(const std::string& key, int seconds);
    int ttl(const std::string& key);
    
    // Transaction 操作
    bool multi();
    bool exec();
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace um
```

### 阶段四：Qt 适配器设计

**adapters/qt/include/um/qt/qt_client.hpp：**

```cpp
#pragma once
#include <QString>
#include <QByteArray>
#include <QMap>
#include <QVector>
#include <optional>
#include "um/client.hpp"

namespace um::qt {

// 前向声明
template<typename T>
using Optional = std::optional<T>;

class QtClient {
public:
    bool connectToServer(const QString& host = "127.0.0.1", int port = 6379);
    void disconnect();
    bool isConnected() const;
    
    // String 操作
    bool set(const QString& key, const QString& value);
    QString get(const QString& key);
    
    // Bytes 操作
    bool bytesSet(const QString& key, const QByteArray& value);
    QByteArray bytesGet(const QString& key);
    
    // Hash 操作
    bool hSet(const QString& key, const QString& field, const QString& value);
    QString hGet(const QString& key, const QString& field);
    QMap<QString, QString> hGetAll(const QString& key);
    
    // List 操作
    bool lPush(const QString& key, const QString& value);
    QString lPop(const QString& key);
    QVector<QString> lRange(const QString& key, int start, int stop);
    
    // Set 操作
    bool sAdd(const QString& key, const QString& value);
    bool sIsMember(const QString& key, const QString& value);
    QVector<QString> sMembers(const QString& key);
    
    // Sorted Set 操作
    bool zAdd(const QString& key, double score, const QString& member);
    
    // Expiration 操作
    bool expire(const QString& key, int seconds);
    int ttl(const QString& key);
    
    // Transaction 操作
    bool multi();
    bool exec();
    
private:
    um::Client coreClient_;  // 标准 C++ 核心客户端
};

} // namespace um::qt
```

### 阶段五：类型转换实现

**adapters/qt/src/qt_client.cpp（关键转换）：**

```cpp
#include "um/qt/qt_client.hpp"
#include <QString>

namespace um::qt {

// QString → std::string
static std::string toStdString(const QString& str) {
    return str.toStdString();
}

// std::string → QString
static QString fromStdString(const std::string& str) {
    return QString::fromStdString(str);
}

// QByteArray → um::Bytes (std::vector<uint8_t>)
static um::Bytes toBytes(const QByteArray& ba) {
    const uint8_t* data = reinterpret_cast<const uint8_t*>(ba.constData());
    return um::Bytes(data, data + ba.size());
}

// um::Bytes → QByteArray
static QByteArray fromBytes(const um::Bytes& bytes) {
    return QByteArray(reinterpret_cast<const char*>(bytes.data()), 
                      static_cast<int>(bytes.size()));
}

bool QtClient::set(const QString& key, const QString& value) {
    return coreClient_.set(toStdString(key), toStdString(value));
}

QString QtClient::get(const QString& key) {
    auto result = coreClient_.get(toStdString(key));
    return result ? fromStdString(*result) : QString();
}

bool QtClient::bytesSet(const QString& key, const QByteArray& value) {
    return coreClient_.setBytes(toStdString(key), toBytes(value));
}

QByteArray QtClient::bytesGet(const QString& key) {
    auto result = coreClient_.getBytes(toStdString(key));
    return result ? fromBytes(*result) : QByteArray();
}

// ... 其他操作的转换实现

} // namespace um::qt
```

---

## 性能分析

### 转换开销对比

| 操作 | 当前实现 | 改造后 | 差异 |
|------|----------|--------|------|
| QString → std::string | 深拷贝（隐式） | 深拷贝（显式） | **相同** |
| QByteArray → std::string | 深拷贝（隐式） | 深拷贝（显式） | **相同** |
| 函数调用层级 | 2 层 | 3 层 | 多一层（可忽略） |

**结论**：改造不会增加性能损失，转换本来就存在。

### 高频场景评估

场景：每秒存储 3 张 PNG 图片（每张 500KB）

| 开销来源 | 耗时 | 占比 |
|----------|------|------|
| 类型转换（500KB） | ~0.05ms | ~1% |
| 网络传输（500KB） | ~5ms | ~98% |
| 其他处理 | ~0.05ms | ~1% |

**结论**：转换开销相对于网络传输可忽略，改造安全。

---

## 迁移路径

### 阶段一：核心提取（1-2 周）

1. 创建 `RedisModule/core/` 目录
2. 将现有操作类改造为 std::string 接口
3. 保持现有 RedisModule 作为兼容层
4. 验证核心库编译通过

### 阶段二：适配器实现（1 周）

1. 创建 `RedisModule/adapters/qt/` 目录
2. 实现 QtClient 到 core::Client 的委托
3. 实现所有类型转换函数
4. 验证 Qt 示例编译运行

### 阶段三：CMake 改造（2-3 天）

1. 添加 `BUILD_QT_ADAPTER` 选项
2. 配置各模块的 CMakeLists.txt
3. 测试不同编译选项组合

### 阶段四：测试与验证（1 周）

1. 为核心库编写 gtest 单元测试
2. 为 Qt 适配器编写测试
3. 验证向后兼容性
4. 性能基准测试对比

### 阶段五：文档更新（2-3 天）

1. 更新 PROJECT_ARCHITECTURE.md
2. 更新 DEPLOYMENT.md
3. 编写迁移指南
4. 更新示例代码

---

## 使用方式

### 纯 C++ 项目（后台服务）

```cmake
# CMakeLists.txt
find_package(um_core REQUIRED)

target_link_libraries(my_server PRIVATE um_core)
```

```cpp
// main.cpp
#include <um/client.hpp>

int main() {
    um::Client client;
    client.connect("127.0.0.1", 6379);
    client.set("key", "value");
    return 0;
}
```

### Qt 项目（GUI 应用）

```cmake
# CMakeLists.txt
find_package(um_core REQUIRED)
find_package(um_qt_adapter REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

target_link_libraries(my_app PRIVATE um_qt_adapter Qt5::Widgets)
```

```cpp
// mainwindow.cpp
#include <um/qt/qt_client.hpp>

void MainWindow::onSaveImage() {
    um::qt::QtClient client;
    client.connectToServer();
    
    QByteArray pngData = loadImage();
    client.bytesSet("image:001", pngData);
}
```

---

## 风险与缓解

| 风险 | 可能性 | 影响 | 缓解措施 |
|------|--------|------|----------|
| 向后兼容性破坏 | 低 | 高 | 提供 compat 层，保持原有 API |
| 性能下降 | 极低 | 中 | 基准测试验证，转换逻辑相同 |
| 编译复杂度增加 | 中 | 低 | 提供清晰的 CMake 配置和文档 |
| 开发周期延长 | 中 | 低 | 分阶段实施，保持现有功能可用 |

---

## 总结

### 改造收益

1. **框架解耦**：核心库可用于任何 C++ 项目
2. **测试便利**：单元测试无需 Qt 环境
3. **部署灵活**：后台服务不依赖 Qt
4. **长期维护**：架构清晰，易于扩展

### 关键决策

- **转换不可避免**：redis-plus-plus 只接受标准 C++ 类型
- **显式优于隐式**：适配器模式让架构更清晰
- **性能无损失**：改造只是移动转换代码位置
- **可选编译**：CMake 选项控制 Qt 依赖

---

## 参考

- [PROJECT_ARCHITECTURE.md](./PROJECT_ARCHITECTURE.md) - 当前架构文档
- [DEPLOYMENT.md](./DEPLOYMENT.md) - 部署说明
- redis-plus-plus 官方文档：https://github.com/sewenew/redis-plus-plus
