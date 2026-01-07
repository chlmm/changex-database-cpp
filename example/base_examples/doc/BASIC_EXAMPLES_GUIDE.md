# Qt Redis Plus Plus Extended 示例项目

本示例项目通过全面的示例演示了如何使用 Qt Redis Plus Plus Extended 库。

## 前置要求

- Qt 5.12 或更高版本
- Redis 服务器运行在 `127.0.0.1:6379`
- redis-plus-plus 库
- hiredis 库

## 构建

在项目根目录执行：

```bash
mkdir build
cd build
cmake ..
make
```

编译后的可执行文件位于 `build/example/example`。

## 使用方法

### 交互式模式

不带参数运行示例程序，进入交互式菜单模式：

```bash
./example
```

你将看到包含可用示例的菜单：

```
Qt Redis Plus Plus Extended Demo
=================================
Available Examples:
1. String Operations - Basic Redis string operations
2. Hash Operations - Demonstrate Redis Hash functionality
3. List Operations - Demonstrate Redis List functionality
4. Set Operations - Demonstrate Redis Set functionality
5. Sorted Set Operations - Demonstrate Redis Sorted Set functionality
6. Expiration Operations - Demonstrate key expiration
7. User Profile Management (Real-World Example)
8. Game Leaderboard (Real-World Example)
9. Run All Demos
0. Exit

Select demo to run (0-9):
```

### 命令行模式

你也可以直接使用命令行参数运行特定示例：

```bash
# 通过名称或编号运行特定示例
./example string        # 或 ./example 1
./example hash          # 或 ./example 2
./example list          # 或 ./example 3
./example set           # 或 ./example 4
./example sorted_set    # 或 ./example 5
./example expiration    # 或 ./example 6
./example user_profile  # 或 ./example 7
./example leaderboard   # 或 ./example 8

# 运行所有示例
./example all           # 或 ./example 9

# 显示帮助信息
./example --help
```

## 示例说明

### 基础操作

#### 1. String Operations (`basic_string_demo.cpp`)
演示基本的 Redis 字符串操作：
- 设置和获取值
- 更新现有键
- 检查键是否存在
- 删除键

#### 2. Hash Operations (`hash_demo.cpp`)
演示 Redis Hash 操作：
- 创建和管理哈希字段
- 获取所有字段和值
- 获取特定字段值
- 检查字段是否存在
- 更新和删除字段

#### 3. List Operations (`list_demo.cpp`)
演示 Redis List 操作：
- 从左侧和右侧推入元素
- 从两端弹出元素
- 获取列表范围
- 通过索引访问元素
- 将列表用作队列

#### 4. Set Operations (`set_demo.cpp`)
演示 Redis Set 操作：
- 添加和删除成员
- 检查成员资格
- 计算集合运算（并集、交集、差集）
- 自动处理重复项

#### 5. Sorted Set Operations (`sorted_set_demo.cpp`)
演示 Redis Sorted Set 操作：
- 添加带分数的成员
- 检索排名成员
- 获取成员分数
- 查询排名
- 动态更新分数

#### 6. Expiration Operations (`expiration_demo.cpp`)
演示 Redis 键过期操作：
- 设置 TTL（生存时间）
- 检查 TTL 值
- 移除过期时间
- 管理临时数据

### 实际应用示例

#### 7. User Profile Management (`user_profile_example.cpp`)
一个实用的示例，展示如何使用 Redis Hash 管理用户档案：
- 创建包含多个字段的用户档案
- 独立更新特定用户字段
- 批量检索用户信息
- 管理用户生命周期（创建、更新、删除）
- 真实的用户数据模型

**核心要点：**
- Redis Hash 非常适合存储类似对象的数据
- 每个字段可以独立更新
- 高效的内存利用率
- 快速访问特定字段

#### 8. Game Leaderboard (`leaderboard_example.cpp`)
一个实用的示例，展示如何使用 Redis Sorted Set 实现实时游戏排行榜：
- 初始化玩家分数
- 查询前 N 名玩家
- 获取玩家排名
- 排行榜分页范围查询
- 实时分数更新
- 查找指定分数范围内的玩家

**核心要点：**
- Sorted Set 自动维护排序
- 排名计算时间为 O(log N)
- 更新是即时的
- 无需重新计算排名
- 非常适合实时评分系统

## 项目结构

```
example/
├── CMakeLists.txt              # 构建配置
├── main.cpp                    # 带菜单系统的主程序
├── demos/                      # 示例实现
│   ├── basic_string_demo.h/.cpp
│   ├── hash_demo.h/.cpp
│   ├── list_demo.h/.cpp
│   ├── set_demo.h/.cpp
│   ├── sorted_set_demo.h/.cpp
│   ├── expiration_demo.h/.cpp
│   ├── user_profile_example.h/.cpp
│   └── leaderboard_example.h/.cpp
└── README.md                   # 本文件
```

## 设计说明

每个示例都实现为一个独立的类，包含：
- `name()` - 返回示例名称
- `description()` - 返回简要描述
- `run()` - 执行示例逻辑

这种模块化设计使得：
- 添加新示例变得简单
- 可以独立运行特定示例
- 维护和更新各个示例
- 隔离理解每个示例

## 提示

1. **确保 Redis 正在运行** - 运行示例之前
2. **使用命令行模式** - 用于自动化或测试
3. **使用交互式模式** - 用于学习和探索
4. **运行所有示例** - 一次查看全部功能
5. **查看源代码** - 了解实现细节
6. **修改示例** - 测试你自己的场景

## 故障排除

### 连接 Redis 服务器失败
确保 Redis 正在运行：
```bash
redis-server
```

### 连接被拒绝
检查 Redis 配置，确保它正在监听 127.0.0.1:6379。

### 编译错误
确保安装了所有依赖项：
- Qt 5.12+
- redis-plus-plus
- hiredis

## 贡献

要添加新示例：

1. 在 `demos/` 目录中创建新的头文件和 cpp 文件
2. 实现一个类，包含 `name()`、`description()` 和 `run()` 静态方法
3. 将新文件添加到 `CMakeLists.txt`
4. 在 `main.cpp` 中包含头文件并添加新示例
5. 在本 README 中更新新示例的描述

## 许可证

本示例项目遵循与 Qt Redis Plus Plus Extended 库相同的许可证。
