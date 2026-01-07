# 项目部署说明

## 概述
本项目使用 3rdParty 目录管理第三方依赖库，支持在没有安装 hiredis 和 redis-plus-plus 的环境中运行。

## 目录结构
```
project/
├── 3rdParty/                    # 第三方库（不纳入版本控制）
│   ├── hiredis/
│   │   ├── include/
│   │   └── lib/
│   └── redis-plus-plus/
│       ├── include/
│       └── lib/
├── build/
│   └── example/
│       ├── example              # 可执行文件
│       ├── libhiredis.so*       # 依赖库
│       └── libredis++.so*       # 依赖库
├── scripts/
│   └── setup_3rdparty.sh       # 初始化脚本
└── .gitignore                   # 忽略 3rdParty 和 build
```

## 初始化（首次使用）

### 1. 在开发机器上设置
```bash
# 从系统复制第三方库到 3rdParty 目录（只需运行一次）
./scripts/setup_3rdparty.sh

# 构建
mkdir -p build
cd build
cmake ..
make
```

### 2. 打包部署
```bash
# 打包整个项目（包含 3rdParty 目录）
tar -czf project.tar.gz --exclude='build/*' .

# 或者只打包可执行文件和依赖库
cd build/example
tar -czf example-package.tar.gz example libhiredis.so* libredis++.so*
```

## 在目标机器上部署

### 方案 1：完整项目部署
```bash
# 解压完整项目
tar -xzf project.tar.gz
cd project

# 构建（因为 3rdParty 已包含，无需 setup）
mkdir -p build && cd build
cmake ..
make

# 运行
cd example
./example
```

### 方案 2：仅部署可执行文件（推荐）
```bash
# 解压可执行文件包
mkdir -p deploy
cd deploy
tar -xzf example-package.tar.gz

# 直接运行
./example
```

## 注意事项

### 1. 系统兼容性
- 目标机器的 Linux 内核版本和 glibc 版本需要与开发机器兼容
- 检查方式：`ldd --version`

### 2. 库文件版本
- 3rdParty 中的库文件已复制自 `/usr/local/lib` 和 `/usr/lib/x86_64-linux-gnu/`
- 确保 hiredis 版本与编译时使用的版本一致
- 当前版本：libhiredis.so.1.1.0

### 3. 运行时依赖
- 可执行文件需要 `libhiredis.so.1.1.0` 和 `libredis++.so.1` 等库文件
- 这些库文件已自动复制到 `build/example/` 目录
- 使用 `$ORIGIN` RPATH，程序会自动在同目录查找库

### 4. 调试运行时错误
```bash
# 检查缺少的库
ldd ./example

# 查看详细的加载信息
LD_DEBUG=libs ./example
```

## 常见问题

### Q1: 运行时报错 "error while loading shared libraries"
**原因**: 缺少依赖库文件或版本不匹配
**解决**:
- 确保 3rdParty 目录存在并包含所有库文件
- 运行 `./scripts/setup_3rdparty.sh` 重新设置
- 检查 `ldd ./example` 的输出

### Q2: 编译时报错找不到头文件
**原因**: 3rdParty 目录未正确设置
**解决**:
```bash
./scripts/setup_3rdparty.sh
rm -rf build
mkdir build && cd build
cmake ..
```

### Q3: 每次构建都重新复制库文件
**现状**: 优化后的构建系统只在库文件缺失时才复制
**验证**: 首次构建后会显示复制消息，后续构建会跳过

## 技术细节

### CMake 配置
- `CMakeLists.txt`: 配置 3rdParty 的 include 和 lib 路径
- `example/CMakeLists.txt`: 构建后自动复制库文件到输出目录
- `cmake/CopyIfMissing.cmake`: 检查并复制缺失的库文件

### RPATH 设置
- RedisModule: `$ORIGIN/../../3rdParty/hiredis/lib:$ORIGIN/../../3rdParty/redis-plus-plus/lib`
- example: `$ORIGIN`（当前目录）

### 符号链接
- `libhiredis.so` → `libhiredis.so.1` → `libhiredis.so.1.1.0`
- `libredis++.so` → `libredis++.so.1` → `libredis++.so.1.3.15`

## 版本信息
- hiredis: 1.1.0
- redis-plus-plus: 1.3.15
- Qt: 5.12.12
- C++: 17
- CMake: 3.14+
