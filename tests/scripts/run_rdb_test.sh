#!/bin/bash
#
# RDB 持久化测试脚本
# 流程：启用 RDB -> 写入数据 -> 触发保存 -> 重启 -> 验证数据
#

set -e

# 加载库
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/common.sh"

# 重新设置 SCRIPT_DIR（因为 common.sh 可能覆盖它）
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
source "${SCRIPT_DIR}/lib/redis_utils.sh"

# 测试配置
TEST_DATA_FILE="/tmp/rdb_test_data.json"
TEST_RESULT_FILE="/tmp/rdb_test_result.json"
TEST_COUNT=10000

# 清理旧数据文件
rm -f "$TEST_DATA_FILE" "$TEST_RESULT_FILE"

# 显示标题
echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    RDB 持久化测试${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 检查 Redis
check_redis || exit 1

# 备份配置
backup_redis_config

# 测试步骤
enable_rdb "60 1"
flush_redis

# 写入数据
log_info "写入 $TEST_COUNT 条测试数据..."
PERSISTENCE_TEST_DATA_FILE="$TEST_DATA_FILE" \
    "${BUILD_DIR}/tests/tst_rdbpersistence" testWriteData

KEYS_BEFORE=$(redis-cli KEYS "rdb_test_*" 2>/dev/null | wc -l | tr -d ' ')
log_info "写入完成，共 $KEYS_BEFORE 条 key"

# 删除旧的 RDB 文件，确保触发真实的 BGSAVE
RDB_FILE=$(get_rdb_file)
rm -f "$RDB_FILE" 2>/dev/null

# 触发 RDB 保存并记录时间（只捕获最后一行的时间值）
SAVE_TIME_MS=$(trigger_bgsave 2>&1 | tail -1)
# 检查是否是有效的数字，如果不是则设为 0
if ! [[ "$SAVE_TIME_MS" =~ ^[0-9]+$ ]]; then
    SAVE_TIME_MS="0"
fi

# 检查 RDB 文件
RDB_SIZE=$(check_file "$RDB_FILE")
if [ "$RDB_SIZE" -gt 0 ]; then
    log_success "RDB 文件已生成: $RDB_FILE ($RDB_SIZE bytes)"
else
    log_error "RDB 文件未找到"
    restore_redis_config
    exit 1
fi

# 重启 Redis 并记录加载时间（只捕获最后一行的时间值）
LOAD_TIME_MS=$(restart_redis 2>&1 | tail -1)
if ! [[ "$LOAD_TIME_MS" =~ ^[0-9]+$ ]]; then
    LOAD_TIME_MS="0"
fi

# 验证数据
log_info "验证数据完整性..."
PERSISTENCE_TEST_DATA_FILE="$TEST_DATA_FILE" \
    "${BUILD_DIR}/tests/tst_rdbpersistence" testVerifyData > "$TEST_RESULT_FILE" 2>&1

# 解析结果
if [ -f "$TEST_RESULT_FILE" ]; then
    # 提取 RESULT: 后面的 JSON 部分
    JSON_RESULT=$(grep "RESULT:" "$TEST_RESULT_FILE" | sed 's/.*RESULT: //')
    RESULT=$(echo "$JSON_RESULT" | grep -o '"result"[[:space:]]*:[[:space:]]*"[^"]*"' | cut -d'"' -f4)
    MATCHED=$(echo "$JSON_RESULT" | grep -o '"matched"[[:space:]]*:[[:space:]]*[0-9]*' | grep -o '[0-9]*')
    TOTAL=$(echo "$JSON_RESULT" | grep -o '"total"[[:space:]]*:[[:space:]]*[0-9]*' | grep -o '[0-9]*')
    
    if [ "$RESULT" = "PASS" ]; then
        log_success "RDB 测试通过: $MATCHED/$TOTAL 条数据验证成功"
        TEST_RESULT="PASS"
    else
        log_error "RDB 测试失败: 数据不一致"
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
echo -e "${BLUE}    RDB 测试结果: ${TEST_RESULT}${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 清理临时文件
rm -f "$TEST_DATA_FILE" "$TEST_RESULT_FILE"

# 输出性能统计
echo "" >&2
echo -e "${BLUE}----------------------------------------${NC}" >&2
echo -e "${BLUE}    RDB 性能统计${NC}" >&2
echo -e "${BLUE}----------------------------------------${NC}" >&2
echo -e "  保存时间: ${GREEN}${SAVE_TIME_MS}ms${NC}" >&2
echo -e "  加载时间: ${GREEN}${LOAD_TIME_MS}ms${NC}" >&2
echo -e "  总耗时:   ${GREEN}$((SAVE_TIME_MS + LOAD_TIME_MS))ms${NC}" >&2
echo -e "${BLUE}----------------------------------------${NC}" >&2
echo "" >&2

# 输出时间到标准输出（供提取，不带颜色）
echo "RDB_SAVE_TIME:${SAVE_TIME_MS}"
echo "RDB_LOAD_TIME:${LOAD_TIME_MS}"

# 返回结果
if [ "$TEST_RESULT" = "PASS" ]; then
    exit 0
else
    exit 1
fi
