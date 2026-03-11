#!/bin/bash
# scripts/build_dependencies.sh
# Clone 第三方依赖源码并编译（不安装到系统）

set -e

# 版本号
HIREDIS_VERSION="v1.3.0"
REDIS_PLUS_PLUS_VERSION="1.3.15"

# 重试次数
MAX_RETRIES=3
RETRY_DELAY=5

# 目录设置
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SOURCE_DIR="${PROJECT_ROOT}/3rdSourceCode"

# 带重试的 git clone 函数
git_clone_with_retry() {
    local repo_url=$1
    local target_dir=$2
    local version=$3
    local retry=0
    
    while [ $retry -lt $MAX_RETRIES ]; do
        echo "尝试克隆 (${retry}/${MAX_RETRIES}): $repo_url"
        if git clone --branch "$version" "$repo_url" "$target_dir"; then
            echo "克隆成功: $repo_url ($version)"
            return 0
        fi
        retry=$((retry + 1))
        echo "克隆失败，等待 ${RETRY_DELAY} 秒后重试..."
        sleep $RETRY_DELAY
    done
    
    echo "错误: 克隆失败 $repo_url"
    return 1
}

# 编译函数（只到 make）
build_only() {
    local name=$1
    local version=$2
    
    echo ""
    echo "============================================"
    echo "Building ${name} ${version}..."
    echo "============================================"
    
    mkdir -p build
    cd build
    
    echo "Running cmake..."
    cmake ..
    
    echo "Running make..."
    make -j$(nproc)
    
    echo "${name} ${version} 编译成功！"
}

echo "============================================"
echo "编译第三方依赖"
echo "============================================"
echo "hiredis version: ${HIREDIS_VERSION}"
echo "redis-plus-plus version: ${REDIS_PLUS_PLUS_VERSION}"
echo "Source directory: ${SOURCE_DIR}"
echo "Max retries: ${MAX_RETRIES}"
echo ""

# 创建目录
mkdir -p "${SOURCE_DIR}"
cd "${SOURCE_DIR}"

# 编译 hiredis
if [ ! -d "hiredis" ]; then
    git_clone_with_retry https://github.com/redis/hiredis.git hiredis ${HIREDIS_VERSION}
fi

cd "${SOURCE_DIR}/hiredis"
if [ ! -d "build" ] || [ ! -f "build/libhiredis.so" ]; then
    build_only "hiredis" "${HIREDIS_VERSION}"
else
    echo "hiredis 已编译，跳过"
fi

cd "${SOURCE_DIR}"

# 编译 redis-plus-plus
if [ ! -d "redis-plus-plus" ]; then
    git_clone_with_retry https://github.com/sewenew/redis-plus-plus.git redis-plus-plus ${REDIS_PLUS_PLUS_VERSION}
fi

cd "${SOURCE_DIR}/redis-plus-plus"
if [ ! -d "build" ] || [ ! -f "build/libredis++.so" ]; then
    build_only "redis-plus-plus" "${REDIS_PLUS_PLUS_VERSION}"
else
    echo "redis-plus-plus 已编译，跳过"
fi

echo ""
echo "============================================"
echo "编译完成！"
echo "============================================"
echo ""

# 交互式询问是否运行 setup_3rdparty.sh
read -p "是否立即复制库文件到 3rdParty 目录? [y/N] " choice
case "$choice" in
    y|Y )
        echo ""
        echo "正在运行 setup_3rdparty.sh..."
        "${SCRIPT_DIR}/setup_3rdparty.sh"
        ;;
    * )
        echo ""
        echo "跳过。稍后可手动运行："
        echo "  ${SCRIPT_DIR}/setup_3rdparty.sh"
        ;;
esac
echo ""
