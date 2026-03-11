#!/bin/bash
#
# 持久化测试统一入口
# 运行 RDB 和 AOF 所有测试
#

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
set +e  # 关闭严格模式，允许测试失败继续运行
RDB_OUTPUT=$("${SCRIPT_DIR}/run_rdb_test.sh" 2>&1)
RDB_EXIT_CODE=$?
set -e  # 重新启用严格模式
if [ $RDB_EXIT_CODE -eq 0 ]; then
    RDB_RESULT="PASS"
    # 从输出中提取时间
    RDB_SAVE_TIME=$(echo "$RDB_OUTPUT" | grep "^RDB_SAVE_TIME:" | cut -d: -f2)
    RDB_LOAD_TIME=$(echo "$RDB_OUTPUT" | grep "^RDB_LOAD_TIME:" | cut -d: -f2)
else
    RDB_RESULT="FAIL"
fi

echo ""

# 运行 AOF 测试
log_info "开始 AOF 测试..."
set +e  # 关闭严格模式，允许测试失败继续运行
AOF_OUTPUT=$("${SCRIPT_DIR}/run_aof_test.sh" 2>&1)
AOF_EXIT_CODE=$?
set -e  # 重新启用严格模式
if [ $AOF_EXIT_CODE -eq 0 ]; then
    AOF_RESULT="PASS"
    # 从输出中提取时间
    AOF_SYNC_TIME=$(echo "$AOF_OUTPUT" | grep "^AOF_SYNC_TIME:" | cut -d: -f2)
    AOF_LOAD_TIME=$(echo "$AOF_OUTPUT" | grep "^AOF_LOAD_TIME:" | cut -d: -f2)
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
    echo -e "  保存: ${RDB_SAVE_TIME:-N/A}ms, 加载: ${RDB_LOAD_TIME:-N/A}ms"
else
    echo -e "RDB 测试: ${RED}失败 ✗${NC}"
fi

if [ "$AOF_RESULT" = "PASS" ]; then
    echo -e "AOF 测试: ${GREEN}通过 ✓${NC}"
    echo -e "  同步: ${AOF_SYNC_TIME:-N/A}ms, 加载: ${AOF_LOAD_TIME:-N/A}ms"
else
    echo -e "AOF 测试: ${RED}失败 ✗${NC}"
fi

echo ""
echo -e "${BLUE}========================================${NC}"

# 保存结果到文件（供报告生成器使用）
cat > "${REPORT_DIR}/persistence_results.json" << EOF
{
    "rdb_result": "${RDB_RESULT}",
    "rdb_save_time_ms": ${RDB_SAVE_TIME:-0},
    "rdb_load_time_ms": ${RDB_LOAD_TIME:-0},
    "aof_result": "${AOF_RESULT}",
    "aof_sync_time_ms": ${AOF_SYNC_TIME:-0},
    "aof_load_time_ms": ${AOF_LOAD_TIME:-0},
    "timestamp": "$(date -Iseconds)"
}
EOF

# 返回整体结果
if [ "$RDB_RESULT" = "PASS" ] && [ "$AOF_RESULT" = "PASS" ]; then
    exit 0
else
    exit 1
fi
