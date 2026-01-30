#!/bin/bash
#
# AOF 持久化测试脚本
# 流程：启用 AOF -> 写入数据 -> 重启 -> 验证数据
#

set -e

# 加载库
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/common.sh"

# 重新设置 SCRIPT_DIR（因为 common.sh 可能覆盖它）
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/redis_utils.sh"

# 测试配置
TEST_DATA_FILE="/tmp/aof_test_data.json"
TEST_RESULT_FILE="/tmp/aof_test_result.json"
TEST_COUNT=1000

# 清理旧数据文件
rm -f "$TEST_DATA_FILE" "$TEST_RESULT_FILE"

# 显示标题
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    AOF 持久化测试${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 检查 Redis
check_redis || exit 1

# 备份配置
backup_redis_config

# 测试步骤
enable_aof
flush_redis

# 写入数据
log_info "写入 $TEST_COUNT 条测试数据..."
PERSISTENCE_TEST_DATA_FILE="$TEST_DATA_FILE" \
    "${BUILD_DIR}/tests/tst_aofpersistence" testWriteData

KEYS_BEFORE=$(redis-cli KEYS "aof_test_*" 2>/dev/null | wc -l | tr -d ' ')
log_info "写入完成，共 $KEYS_BEFORE 条 key"

# 检查 AOF 文件
AOF_FILE=$(get_aof_file)
AOF_SIZE=$(check_file "$AOF_FILE")
if [ "$AOF_SIZE" -gt 0 ]; then
    log_success "AOF 文件已生成: $AOF_FILE ($AOF_SIZE bytes)"
else
    log_warn "AOF 文件可能尚未生成，继续测试..."
fi

# 重启 Redis（带 AOF）
restart_redis "--appendonly yes"

# 验证数据
log_info "验证数据完整性..."
PERSISTENCE_TEST_DATA_FILE="$TEST_DATA_FILE" \
    "${BUILD_DIR}/tests/tst_aofpersistence" testVerifyData > "$TEST_RESULT_FILE" 2>&1

# 解析结果
if [ -f "$TEST_RESULT_FILE" ]; then
    # 提取 RESULT: 后面的 JSON 部分
    JSON_RESULT=$(grep "RESULT:" "$TEST_RESULT_FILE" | sed 's/.*RESULT: //')
    RESULT=$(echo "$JSON_RESULT" | grep -o '"result"[[:space:]]*:[[:space:]]*"[^"]*"' | cut -d'"' -f4)
    MATCHED=$(echo "$JSON_RESULT" | grep -o '"matched"[[:space:]]*:[[:space:]]*[0-9]*' | grep -o '[0-9]*')
    TOTAL=$(echo "$JSON_RESULT" | grep -o '"total"[[:space:]]*:[[:space:]]*[0-9]*' | grep -o '[0-9]*')
    
    if [ "$RESULT" = "PASS" ]; then
        log_success "AOF 测试通过: $MATCHED/$TOTAL 条数据验证成功"
        TEST_RESULT="PASS"
    else
        log_error "AOF 测试失败: 数据不一致"
        TEST_RESULT="FAIL"
    fi
else
    log_error "测试结果文件未生成"
    TEST_RESULT="FAIL"
fi

# 恢复配置
restore_redis_config

# 输出最终报告
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    AOF 测试结果: ${TEST_RESULT}${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 清理临时文件
rm -f "$TEST_DATA_FILE" "$TEST_RESULT_FILE"

# 返回结果
if [ "$TEST_RESULT" = "PASS" ]; then
    exit 0
else
    exit 1
fi
