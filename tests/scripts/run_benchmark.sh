#!/bin/bash
#
# 性能基准测试执行脚本
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/common.sh"

BUILD_DIR="${PROJECT_DIR}/build"
RESULT_DIR="${REPORT_DIR}"

# 创建结果目录
mkdir -p "${RESULT_DIR}"

log_info "开始性能基准测试..."

# 检查测试程序
if [ ! -f "${BUILD_DIR}/tests/tst_stringbenchmark" ] || [ ! -f "${BUILD_DIR}/tests/tst_bytesbenchmark" ]; then
    log_error "测试程序未找到，请先编译项目"
    exit 1
fi

# 运行 String Benchmark
log_info "运行 String 基准测试..."
"${BUILD_DIR}/tests/tst_stringbenchmark" -maxwarnings 0 > "${RESULT_DIR}/string_benchmark.log" 2>&1 || true
log_success "String 测试完成"

# 运行 Bytes Benchmark
log_info "运行 Bytes 基准测试..."
"${BUILD_DIR}/tests/tst_bytesbenchmark" -maxwarnings 0 > "${RESULT_DIR}/bytes_benchmark.log" 2>&1 || true
log_success "Bytes 测试完成"

log_success "性能基准测试完成！"
