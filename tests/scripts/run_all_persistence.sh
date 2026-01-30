#!/bin/bash
#
# 持久化测试统一入口
# 运行 RDB 和 AOF 所有测试
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/common.sh"
# 重新设置 SCRIPT_DIR，因为 common.sh 会覆盖它
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# 结果汇总
RDB_RESULT="SKIP"
AOF_RESULT="SKIP"

# 显示标题
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    持久化测试套件${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 运行 RDB 测试
log_info "开始 RDB 测试..."
if "${SCRIPT_DIR}/run_rdb_test.sh"; then
    RDB_RESULT="PASS"
else
    RDB_RESULT="FAIL"
fi

echo ""

# 运行 AOF 测试
log_info "开始 AOF 测试..."
if "${SCRIPT_DIR}/run_aof_test.sh"; then
    AOF_RESULT="PASS"
else
    AOF_RESULT="FAIL"
fi

# 汇总输出
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    持久化测试汇总${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

if [ "$RDB_RESULT" = "PASS" ]; then
    echo -e "RDB 测试: ${GREEN}通过 ✓${NC}"
else
    echo -e "RDB 测试: ${RED}失败 ✗${NC}"
fi

if [ "$AOF_RESULT" = "PASS" ]; then
    echo -e "AOF 测试: ${GREEN}通过 ✓${NC}"
else
    echo -e "AOF 测试: ${RED}失败 ✗${NC}"
fi

echo ""
echo -e "${BLUE}========================================${NC}"

# 保存结果到文件（供报告生成器使用）
cat > "${REPORT_DIR}/persistence_results.json" << EOF
{
    "rdb_result": "${RDB_RESULT}",
    "aof_result": "${AOF_RESULT}",
    "timestamp": "$(date -Iseconds)"
}
EOF

# 返回整体结果
if [ "$RDB_RESULT" = "PASS" ] && [ "$AOF_RESULT" = "PASS" ]; then
    exit 0
else
    exit 1
fi
