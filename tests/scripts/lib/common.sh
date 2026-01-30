#!/bin/bash
#
# 公共库 - 颜色定义、变量、工具函数
#

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'  # No Color

# 路径变量
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTS_DIR="$(dirname "$(dirname "$SCRIPT_DIR")")"
PROJECT_DIR="$(dirname "$TESTS_DIR")"
BUILD_DIR="${PROJECT_DIR}/build"
REPORT_DIR="${BUILD_DIR}/reports"

# 确保 build 和报告目录存在
mkdir -p "${BUILD_DIR}"
mkdir -p "${REPORT_DIR}"

# 日志函数
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[PASS]${NC} $1"
}

log_error() {
    echo -e "${RED}[FAIL]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

# 检查 Redis 连接
check_redis() {
    if ! redis-cli ping > /dev/null 2>&1; then
        log_error "Redis 未运行"
        return 1
    fi
    log_success "Redis 连接正常"
    return 0
}

# 保存测试结果到 JSON 文件
save_test_result() {
    local test_name=$1
    local result=$2
    local details=$3
    local output_file=$4
    
    cat > "$output_file" << EOF
{
    "test_name": "$test_name",
    "result": "$result",
    "details": $details,
    "timestamp": "$(date -Iseconds)"
}
EOF
}

# 读取测试结果
load_test_result() {
    local input_file=$1
    if [ -f "$input_file" ]; then
        cat "$input_file"
    else
        echo "{}"
    fi
}
