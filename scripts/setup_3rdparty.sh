#!/bin/bash
# scripts/setup_3rdparty.sh
# 从编译目录复制库文件和头文件到 3rdParty（无需安装到系统）

set -e

# 目录设置
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SOURCE_DIR="${PROJECT_ROOT}/3rdSourceCode"
THIRD_PARTY_DIR="${PROJECT_ROOT}/3rdParty"

echo "============================================"
echo "设置 3rdParty 库文件"
echo "============================================"
echo "Source: ${SOURCE_DIR}"
echo "Target: ${THIRD_PARTY_DIR}"
echo ""

# 创建目录结构
mkdir -p "${THIRD_PARTY_DIR}/hiredis/"{lib,include/hiredis/adapters}
mkdir -p "${THIRD_PARTY_DIR}/redis-plus-plus/"{lib,include/sw/redis++/patterns}

# ========== hiredis ==========
echo "[1/2] 复制 hiredis 文件..."

# 复制库文件
if [ -d "${SOURCE_DIR}/hiredis/build" ]; then
    cp -P ${SOURCE_DIR}/hiredis/build/libhiredis.so* "${THIRD_PARTY_DIR}/hiredis/lib/" 2>/dev/null || true
    echo "  库文件: $(ls ${THIRD_PARTY_DIR}/hiredis/lib/)"
else
    echo "  错误: hiredis 未编译，请先运行 build_dependencies.sh"
    exit 1
fi

# 复制头文件
cp ${SOURCE_DIR}/hiredis/*.h "${THIRD_PARTY_DIR}/hiredis/include/hiredis/" 2>/dev/null || true
cp ${SOURCE_DIR}/hiredis/adapters/*.h "${THIRD_PARTY_DIR}/hiredis/include/hiredis/adapters/" 2>/dev/null || true
echo "  头文件: hiredis/*.h, hiredis/adapters/*.h"

# ========== redis-plus-plus ==========
echo "[2/2] 复制 redis-plus-plus 文件..."

# 复制库文件
if [ -d "${SOURCE_DIR}/redis-plus-plus/build" ]; then
    cp -P ${SOURCE_DIR}/redis-plus-plus/build/libredis++.so* "${THIRD_PARTY_DIR}/redis-plus-plus/lib/" 2>/dev/null || true
    cp ${SOURCE_DIR}/redis-plus-plus/build/libredis++.a "${THIRD_PARTY_DIR}/redis-plus-plus/lib/" 2>/dev/null || true
    echo "  库文件: $(ls ${THIRD_PARTY_DIR}/redis-plus-plus/lib/)"
else
    echo "  错误: redis-plus-plus 未编译，请先运行 build_dependencies.sh"
    exit 1
fi

# 复制头文件
cp -r ${SOURCE_DIR}/redis-plus-plus/src/sw/redis++/*.h "${THIRD_PARTY_DIR}/redis-plus-plus/include/sw/redis++/" 2>/dev/null || true
cp -r ${SOURCE_DIR}/redis-plus-plus/src/sw/redis++/patterns/*.h "${THIRD_PARTY_DIR}/redis-plus-plus/include/sw/redis++/patterns/" 2>/dev/null || true
echo "  头文件: sw/redis++/*.h, sw/redis++/patterns/*.h"

echo ""
echo "============================================"
echo "3rdParty 设置完成！"
echo "============================================"
echo ""
echo "目录结构:"
tree -L 3 "${THIRD_PARTY_DIR}" 2>/dev/null || find "${THIRD_PARTY_DIR}" -type f | head -20
echo ""
