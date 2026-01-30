#!/bin/bash
#
# Markdown 报告生成器
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/common.sh"

# 参数解析
WITH_PERSISTENCE=false
if [ "$1" = "--with-persistence" ]; then
    WITH_PERSISTENCE=true
fi

# 时间戳
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
REPORT_FILE="${REPORT_DIR}/benchmark_report_${TIMESTAMP}.md"

log_info "生成 Markdown 报告..."

# 提取性能测试数据
extract_benchmark_data() {
    local file=$1
    if [ ! -f "$file" ]; then
        return
    fi
    
    local content=$(cat "$file")
    echo "$content" | sed -n '/RESULT/{N;s/\n/ /;p}' | while read -r line; do
        test_name=$(echo "$line" | sed -n 's/.*::\([^:]*\)():.*/\1/p')
        avg_time=$(echo "$line" | sed -n 's/.* \([0-9]\+\.\?[0-9]*\) msecs per iteration.*/\1/p')
        iterations=$(echo "$line" | sed -n 's/.*iterations: \([0-9]\+\)).*/\1/p')
        
        if [ -n "$test_name" ] && [ -n "$avg_time" ]; then
            echo "${test_name}|${avg_time}|${iterations}"
        fi
    done
}

# 推断数据大小
get_data_size() {
    local test_name=$1
    case "$test_name" in
        *1KB*) echo "1KB" ;;
        *10KB*) echo "10KB" ;;
        *100KB*) echo "100KB" ;;
        *1MB*) echo "1MB" ;;
        *) echo "-" ;;
    esac
}

# 推断操作类型
get_op_type() {
    local test_name=$1
    case "$test_name" in
        *Create*|*Set*) echo "Create" ;;
        *Read*|*Get*) echo "Read" ;;
        *Update*) echo "Update" ;;
        *Delete*|*Del*) echo "Delete" ;;
        *Append*) echo "Append" ;;
        *) echo "-" ;;
    esac
}

# 计算 QPS
calculate_qps() {
    local avg_time=$1
    if [ -n "$avg_time" ] && [ "$avg_time" != "0" ]; then
        awk "BEGIN {printf \"%.0f\", 1000/$avg_time}"
    else
        echo "0"
    fi
}

# 生成性能表格
generate_perf_table() {
    local log_file=$1
    local table=""
    
    while IFS='|' read -r test_name avg_time iterations; do
        [ -z "$test_name" ] && continue
        
        local data_size=$(get_data_size "$test_name")
        local op_type=$(get_op_type "$test_name")
        local qps=$(calculate_qps "$avg_time")
        
        table="${table}| ${op_type} | ${data_size} | ${avg_time} ms | ${iterations} | ~${qps} ops/sec |\n"
    done < <(extract_benchmark_data "$log_file")
    
    echo -e "$table"
}

# 读取持久化测试结果
RDB_RESULT="未测试"
AOF_RESULT="未测试"
if [ "$WITH_PERSISTENCE" = true ] && [ -f "${REPORT_DIR}/persistence_results.json" ]; then
    RDB_RESULT=$(grep -o '"rdb_result": *"[^"]*"' "${REPORT_DIR}/persistence_results.json" | cut -d'"' -f4 || echo "未知")
    AOF_RESULT=$(grep -o '"aof_result": *"[^"]*"' "${REPORT_DIR}/persistence_results.json" | cut -d'"' -f4 || echo "未知")
fi

# 生成报告
cat > "${REPORT_FILE}" << EOF
# Redis 测试报告

生成时间: $(date '+%Y-%m-%d %H:%M:%S')

## 测试环境

- Redis 版本: $(redis-cli INFO server | grep redis_version | cut -d: -f2 | tr -d '\r')
- 测试工具: Qt Test Framework

EOF

# 添加持久化测试结果
if [ "$WITH_PERSISTENCE" = true ]; then
cat >> "${REPORT_FILE}" << EOF
## 持久化测试

| 测试项 | 结果 |
|:-------|:-----|
| RDB 模式 | ${RDB_RESULT} |
| AOF 模式 | ${AOF_RESULT} |

EOF
fi

# 添加性能测试结果
cat >> "${REPORT_FILE}" << EOF
## 性能基准测试

### String 类型

| 操作类型 | 数据大小 | 单次耗时 | 迭代次数 | QPS 估算 |
|:---------|:---------|:---------|:---------|:---------|
$(generate_perf_table "${REPORT_DIR}/string_benchmark.log")

### Bytes 类型

| 操作类型 | 数据大小 | 单次耗时 | 迭代次数 | QPS 估算 |
|:---------|:---------|:---------|:---------|:---------|
$(generate_perf_table "${REPORT_DIR}/bytes_benchmark.log")

---
*报告自动生成*
EOF

log_success "报告生成完成: ${REPORT_FILE}"

# 显示报告预览
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           报告预览${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""
cat "${REPORT_FILE}"
echo ""
echo -e "${BLUE}========================================${NC}"
