#!/bin/bash

# Redis性能测试报告生成器
# 生成格式化的汇总报告

set -e

TEST_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${TEST_DIR}/../build/tests"
REPORT_DIR="${TEST_DIR}/reports"
mkdir -p "${REPORT_DIR}"

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# 显示模式
SHOW_MODE=""

# 交互式选择菜单
show_mode_menu() {
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}    选择输出模式${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    echo "  1) 显示所有信息（完整日志）"
    echo "  2) 精简信息（仅显示结果汇总）"
    echo ""
    echo -e "${BLUE}========================================${NC}"
    read -p "请选择 [1-2]: " mode_choice
    
    case $mode_choice in
        1) SHOW_MODE="verbose" ;;
        2) SHOW_MODE="minimal" ;;
        *) 
            echo -e "${YELLOW}无效选择，使用精简模式${NC}"
            SHOW_MODE="minimal"
            ;;
    esac
}

# 时间戳
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
REPORT_FILE="${REPORT_DIR}/benchmark_report_${TIMESTAMP}.md"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    Redis性能测试报告生成器${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 检查Redis
echo -e "${YELLOW}检查Redis连接...${NC}"
if ! redis-cli ping > /dev/null 2>&1; then
    echo -e "${RED}错误: Redis未运行${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Redis连接正常${NC}"
echo ""

# 选择显示模式
show_mode_menu

# 运行测试并捕获输出
echo -e "${YELLOW}正在运行性能测试...${NC}"
echo ""

# 创建临时文件存储原始结果
STRING_RESULT=$(mktemp)
BYTES_RESULT=$(mktemp)

# 根据模式决定输出方式
if [ "$SHOW_MODE" = "verbose" ]; then
    # 完整模式：显示所有输出
    echo -e "${CYAN}运行 String 基准测试...${NC}"
    ${BUILD_DIR}/tst_stringbenchmark -maxwarnings 0 2>&1 | tee "${STRING_RESULT}" || true
    
    echo ""
    echo -e "${CYAN}运行 Bytes 基准测试...${NC}"
    ${BUILD_DIR}/tst_bytesbenchmark -maxwarnings 0 2>&1 | tee "${BYTES_RESULT}" || true
else
    # 精简模式：只显示进度，不显示详细日志
    echo -e "${CYAN}运行 String 基准测试...${NC}"
    ${BUILD_DIR}/tst_stringbenchmark -maxwarnings 0 > "${STRING_RESULT}" 2>&1 || true
    echo -e "${GREEN}✓ String 测试完成${NC}"
    
    echo -e "${CYAN}运行 Bytes 基准测试...${NC}"
    ${BUILD_DIR}/tst_bytesbenchmark -maxwarnings 0 > "${BYTES_RESULT}" 2>&1 || true
    echo -e "${GREEN}✓ Bytes 测试完成${NC}"
fi

# 生成报告
echo ""
echo -e "${YELLOW}正在生成格式化报告...${NC}"

# 提取数据并计算QPS的函数
extract_data() {
    local file=$1
    
    # 检查文件是否存在
    if [ ! -f "$file" ]; then
        return
    fi
    
    # 读取整个文件，处理跨行的RESULT
    local content=$(cat "$file")
    
    # 使用sed将RESULT行和下一行合并，然后提取
    echo "$content" | sed -n '/RESULT/{N;s/\n/ /;p}' | while read -r line; do
        # 提取测试名 - 在 :: 和 (): 之间
        test_name=$(echo "$line" | sed -n 's/.*::\([^:]*\)():.*/\1/p')
        
        # 提取单次耗时 - msecs per iteration 前面的数字
        avg_time=$(echo "$line" | sed -n 's/.* \([0-9]\+\.\?[0-9]*\) msecs per iteration.*/\1/p')
        
        # 提取迭代次数 - iterations: 后面的数字
        iterations=$(echo "$line" | sed -n 's/.*iterations: \([0-9]\+\)).*/\1/p')
        
        # 提取总时间 - total: 后面的数字
        total_time=$(echo "$line" | sed -n 's/.*total: \([0-9]\+\.\?[0-9]*\).*/\1/p')
        
        # 输出
        if [ -n "$test_name" ] && [ -n "$avg_time" ]; then
            echo "${test_name}|${avg_time}|${iterations}|${total_time}"
        fi
    done
}

# 生成Markdown报告
cat > "${REPORT_FILE}" << 'EOF'
# Redis性能测试报告

生成时间: TIMESTAMP

## 测试环境

- Redis版本: REDIS_VERSION
- 测试工具: Qt Test Framework
- 测试类型: CRUD基准测试

## 性能汇总

### String 类型测试

| 操作类型 | 数据大小 | 单次耗时 | 迭代次数 | QPS估算 |
|:---------|:---------|:---------|:---------|:--------|
STRING_DATA

### Bytes 类型测试

| 操作类型 | 数据大小 | 单次耗时 | 迭代次数 | QPS估算 |
|:---------|:---------|:---------|:---------|:--------|
BYTES_DATA



---
*报告由 generate_report.sh 自动生成*
EOF

# 替换变量
sed -i "s/TIMESTAMP/$(date '+%Y-%m-%d %H:%M:%S')/g" "${REPORT_FILE}"
sed -i "s/REDIS_VERSION/$(redis-cli INFO server | grep redis_version | cut -d: -f2 | tr -d '\r')/g" "${REPORT_FILE}"

# 处理String数据
STRING_TABLE=""
while IFS='|' read -r test_name avg_time iterations total_time; do
    [ -z "$test_name" ] && continue
    
    # 计算QPS
    if [ -n "$avg_time" ] && [ "$avg_time" != "0" ]; then
        qps=$(awk "BEGIN {printf \"%.0f\", 1000/$avg_time}")
    else
        qps="0"
    fi
    
    # 推断数据大小
    data_size="-"
    if [[ "$test_name" == *"1KB"* ]]; then
        data_size="1KB"
    elif [[ "$test_name" == *"10KB"* ]]; then
        data_size="10KB"
    elif [[ "$test_name" == *"100KB"* ]]; then
        data_size="100KB"
    elif [[ "$test_name" == *"1MB"* ]]; then
        data_size="1MB"
    fi
    
    # 推断操作类型
    op_type="-"
    if [[ "$test_name" == *"Create"* ]] || [[ "$test_name" == *"Set"* ]]; then
        op_type="Create"
    elif [[ "$test_name" == *"Read"* ]] || [[ "$test_name" == *"Get"* ]]; then
        op_type="Read"
    elif [[ "$test_name" == *"Update"* ]]; then
        op_type="Update"
    elif [[ "$test_name" == *"Delete"* ]] || [[ "$test_name" == *"Del"* ]]; then
        op_type="Delete"
    elif [[ "$test_name" == *"Append"* ]]; then
        op_type="Append"
    fi
    
    STRING_TABLE="${STRING_TABLE}| ${op_type} | ${data_size} | ${avg_time} ms | ${iterations} | ~${qps} ops/sec |\n"
done < <(extract_data "${STRING_RESULT}" "string")

if [ -n "$STRING_TABLE" ]; then
    # 使用printf处理转义字符
    STRING_TEMP=$(mktemp)
    printf "%b" "$STRING_TABLE" > "$STRING_TEMP"
    sed -i "/STRING_DATA/r $STRING_TEMP" "${REPORT_FILE}"
    sed -i '/STRING_DATA/d' "${REPORT_FILE}"
    rm "$STRING_TEMP"
else
    sed -i 's/STRING_DATA/| - | - | - | - | - |/g' "${REPORT_FILE}"
fi

# 处理Bytes数据
BYTES_TABLE=""
while IFS='|' read -r test_name avg_time iterations total_time; do
    [ -z "$test_name" ] && continue
    
    # 计算QPS
    if [ -n "$avg_time" ] && [ "$avg_time" != "0" ]; then
        qps=$(awk "BEGIN {printf \"%.0f\", 1000/$avg_time}")
    else
        qps="0"
    fi
    
    # 推断数据大小
    data_size="-"
    if [[ "$test_name" == *"1KB"* ]]; then
        data_size="1KB"
    elif [[ "$test_name" == *"10KB"* ]]; then
        data_size="10KB"
    elif [[ "$test_name" == *"100KB"* ]]; then
        data_size="100KB"
    elif [[ "$test_name" == *"1MB"* ]]; then
        data_size="1MB"
    fi
    
    # 推断操作类型
    op_type="-"
    if [[ "$test_name" == *"Create"* ]] || [[ "$test_name" == *"Set"* ]]; then
        op_type="Create"
    elif [[ "$test_name" == *"Read"* ]] || [[ "$test_name" == *"Get"* ]]; then
        op_type="Read"
    elif [[ "$test_name" == *"Update"* ]]; then
        op_type="Update"
    elif [[ "$test_name" == *"Delete"* ]] || [[ "$test_name" == *"Del"* ]]; then
        op_type="Delete"
    elif [[ "$test_name" == *"Append"* ]]; then
        op_type="Append"
    fi
    
    BYTES_TABLE="${BYTES_TABLE}| ${op_type} | ${data_size} | ${avg_time} ms | ${iterations} | ~${qps} ops/sec |\n"
done < <(extract_data "${BYTES_RESULT}" "bytes")

if [ -n "$BYTES_TABLE" ]; then
    BYTES_TEMP=$(mktemp)
    printf "%b" "$BYTES_TABLE" > "$BYTES_TEMP"
    sed -i "/BYTES_DATA/r $BYTES_TEMP" "${REPORT_FILE}"
    sed -i '/BYTES_DATA/d' "${REPORT_FILE}"
    rm "$BYTES_TEMP"
else
    sed -i 's/BYTES_DATA/| - | - | - | - | - |/g' "${REPORT_FILE}"
fi

# 清理临时文件
rm -f "${STRING_RESULT}" "${BYTES_RESULT}"

echo ""
echo -e "${GREEN}✓ 报告生成完成!${NC}"
echo -e "${CYAN}报告路径: ${REPORT_FILE}${NC}"
echo ""

# 显示报告内容
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           报告预览${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""
cat "${REPORT_FILE}"
echo ""
echo -e "${BLUE}========================================${NC}"

echo ""
echo -e "${GREEN}报告已保存到: ${REPORT_FILE}${NC}"

# 根据模式决定是否显示报告内容
if [ "$SHOW_MODE" = "verbose" ]; then
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}           报告预览${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    cat "${REPORT_FILE}"
    echo ""
    echo -e "${BLUE}========================================${NC}"
else
    # 精简模式：只显示关键摘要
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}           测试摘要${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    
    # 提取关键数据
    echo -e "${CYAN}String 测试:${NC}"
    while IFS='|' read -r test_name avg_time iterations total_time; do
        [ -z "$test_name" ] && continue
        short_name=$(echo "$test_name" | sed 's/benchmarkString//')
        echo "  - ${short_name}: ${avg_time} ms (${iterations}次迭代)"
    done < <(extract_data "${STRING_RESULT}" | head -3)
    
    echo ""
    echo -e "${CYAN}Bytes 测试:${NC}"
    while IFS='|' read -r test_name avg_time iterations total_time; do
        [ -z "$test_name" ] && continue
        short_name=$(echo "$test_name" | sed 's/benchmarkBytes//')
        echo "  - ${short_name}: ${avg_time} ms (${iterations}次迭代)"
    done < <(extract_data "${BYTES_RESULT}" | head -5)
    
    echo ""
    echo -e "${BLUE}========================================${NC}"
fi
