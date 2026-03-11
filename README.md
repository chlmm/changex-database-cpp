# Qt Redis Plus Plus Project

基于 Qt5 和 redis-plus-plus 的 Redis C++ 客户端封装库，提供简洁易用的 Redis 操作接口。

## 特性

- 封装常用的 Redis 操作（String、Hash、List、Set、Sorted Set 等）
- 支持连接池管理
- 支持事务和管道操作
- 完整的单元测试和持久化测试
- 无需安装依赖到系统，项目自包含第三方库

## 依赖项

| 依赖 | 版本 | 说明 |
|------|------|------|
| CMake | >= 3.14 | 构建系统 |
| C++ | >= 17 | 编译标准 |
| Qt | >= 5.12 | GUI 和网络模块（推荐 5.15.x） |
| hiredis | v1.3.0 | Redis C 客户端 |
| redis-plus-plus | 1.3.15 | Redis C++ 封装库 |
| Redis Server | >= 5.0 | Redis 服务端 |

## 快速开始

### 1. 安装依赖

运行以下脚本自动下载并编译依赖：

```bash
./scripts/build_dependencies.sh
```

脚本会：
- 克隆 hiredis (v1.3.0) 和 redis-plus-plus (1.3.15) 到 `3rdSourceCode/`
- 编译生成库文件
- 询问是否复制到 `3rdParty/` 目录

### 2. 安装 Qt

```bash
# Ubuntu/Debian（安装系统默认 Qt5 版本）
sudo apt-get install qt5-default qtbase5-dev

# macOS
brew install qt@5

# 如需特定版本，请从 Qt 官网下载：
# https://download.qt.io/archive/qt/
```

### 3. 构建项目

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 4. 运行测试

```bash
# 确保 Redis 服务运行
redis-cli ping

# 运行单元测试
./tests/tst_redisoperations

# 运行持久化测试
./tests/tst_rdbpersistence
./tests/tst_aofpersistence
```

## 项目结构

```
.
├── CMakeLists.txt              # CMake 构建配置
├── README.md                   # 项目说明
├── cmake/                      # CMake 配置文件
│   ├── 3rdparty.cmake          # 第三方库配置
│   └── build_options.cmake     # 编译选项
├── scripts/                    # 构建脚本
│   ├── build_dependencies.sh   # 依赖编译脚本
│   └── setup_3rdparty.sh       # 库文件复制脚本
├── 3rdParty/                   # 第三方库（不纳入版本控制）
├── 3rdSourceCode/              # 第三方源码（不纳入版本控制）
├── RedisModule/                # Redis 操作封装模块
│   ├── redismanager.h/cpp      # Redis 管理器
│   └── operation/              # 各类型操作封装
├── example/                    # 示例代码
│   ├── base_examples/          # 基础示例
│   └── redis_examples/         # Redis 操作示例
├── tests/                      # 测试代码
│   └── persistence/            # 持久化测试
└── doc/                        # 文档
    ├── PROJECT_OVERVIEW.md     # 项目概述
    ├── PROJECT_ARCHITECTURE.md # 架构设计
    └── DEPLOYMENT.md           # 部署文档
```

## API 示例

### 连接 Redis

```cpp
#include "redismanager.h"

RedisManager manager;
manager.connect("127.0.0.1", 6379);
```

### String 操作

```cpp
manager.set("key", "value");
QString value = manager.get("key");
```

### Hash 操作

```cpp
manager.hSet("user:1", "name", "Alice");
manager.hSet("user:1", "age", "30");
QString name = manager.hGet("user:1", "name");
```

### List 操作

```cpp
manager.lPush("mylist", "item1");
manager.lPush("mylist", "item2");
QString item = manager.lPop("mylist");
```

### Set 操作

```cpp
manager.sAdd("myset", "member1");
bool isMember = manager.sIsMember("myset", "member1");
```

## 构建选项

```bash
# 禁用测试
cmake -DBUILD_TESTS=OFF ..

# 构建示例
cmake -DBUILD_BASE_EXAMPLES=ON -DBUILD_REDIS_EXAMPLES=ON ..
```

## 故障排除

### CMake 找不到 Qt5

设置 Qt 路径：
```bash
# Linux 系统安装
export Qt5_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5

# 或自定义安装路径
export Qt5_DIR=/path/to/Qt/5.15.x/gcc_64/lib/cmake/Qt5
export CMAKE_PREFIX_PATH=/path/to/Qt/5.15.x/gcc_64
```

### 无法连接 Redis

- 确保 Redis 服务运行：`redis-cli ping`
- 检查主机和端口配置（默认：127.0.0.1:6379）

### 链接错误

确保已运行依赖安装脚本：
```bash
./scripts/build_dependencies.sh
./scripts/setup_3rdparty.sh
```

## 许可证

本项目仅供学习和开发目的使用。
