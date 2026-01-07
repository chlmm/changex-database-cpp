#!/bin/bash
# scripts/setup_3rdparty.sh
# 手动脚本：复制第三方库到 3rdParty 目录

set -e

echo "Setting up 3rdParty libraries..."

# 创建目录结构
mkdir -p 3rdParty/{hiredis,redis-plus-plus}/{include,lib}

# 复制 hiredis 库文件
echo "Copying hiredis libraries..."
cp /usr/local/lib/libhiredis.so* 3rdParty/hiredis/lib/
cp /usr/lib/x86_64-linux-gnu/libhiredis.so* 3rdParty/hiredis/lib/ 2>/dev/null || true

# 复制 hiredis 头文件
echo "Copying hiredis headers..."
cp -r /usr/local/include/hiredis 3rdParty/hiredis/include/

# 复制 redis-plus-plus 库文件
echo "Copying redis-plus-plus libraries..."
cp /usr/local/lib/libredis++.so* 3rdParty/redis-plus-plus/lib/

# 复制 redis-plus-plus 头文件
echo "Copying redis-plus-plus headers..."
cp -r /usr/local/include/sw 3rdParty/redis-plus-plus/include/

echo ""
echo "3rdParty libraries copied successfully!"
echo ""
echo "Structure:"
tree -L 3 3rdParty 2>/dev/null || find 3rdParty -type f | head -20
