#!/bin/bash
#
# Redis 工具库 - Redis 配置管理、启停操作
#

# common.sh 应该已经被主脚本加载，这里不再重复加载
# 如果未加载，使用以下路径
if [ -z "$RED" ]; then
    source "$(dirname "${BASH_SOURCE[0]}")/common.sh"
fi

# 原始配置备份
ORIGINAL_SAVE=""
ORIGINAL_AOF=""
ORIGINAL_DIR=""

# 保存当前 Redis 配置
backup_redis_config() {
    log_info "备份 Redis 配置..."
    ORIGINAL_SAVE=$(redis-cli CONFIG GET save | tail -1)
    ORIGINAL_AOF=$(redis-cli CONFIG GET appendonly | tail -1)
    ORIGINAL_DIR=$(redis-cli CONFIG GET dir | tail -1)
    log_info "原始 save: '$ORIGINAL_SAVE'"
    log_info "原始 appendonly: '$ORIGINAL_AOF'"
    log_info "原始 dir: '$ORIGINAL_DIR'"
}

# 恢复原始配置
restore_redis_config() {
    log_info "恢复 Redis 配置..."
    if [ -n "$ORIGINAL_SAVE" ]; then
        redis-cli CONFIG SET save "$ORIGINAL_SAVE" > /dev/null 2>&1
    fi
    if [ -n "$ORIGINAL_AOF" ]; then
        redis-cli CONFIG SET appendonly "$ORIGINAL_AOF" > /dev/null 2>&1
    fi
    if [ -n "$ORIGINAL_DIR" ]; then
        redis-cli CONFIG SET dir "$ORIGINAL_DIR" > /dev/null 2>&1
    fi
    log_success "配置已恢复"
}

# 启用 RDB 模式
enable_rdb() {
    local save_policy=${1:-"60 1"}
    log_info "启用 RDB 模式 (save $save_policy)"
    # 设置工作目录为 build 目录，避免污染源码
    redis-cli CONFIG SET dir "$BUILD_DIR" > /dev/null
    redis-cli CONFIG SET save "$save_policy" > /dev/null
    log_success "RDB 已启用 (工作目录: $BUILD_DIR)"
}

# 启用 AOF 模式
enable_aof() {
    log_info "启用 AOF 模式"
    # 设置工作目录为 build 目录，避免污染源码
    redis-cli CONFIG SET dir "$BUILD_DIR" > /dev/null
    redis-cli CONFIG SET appendonly yes > /dev/null
    sleep 1  # 等待 AOF 文件创建
    log_success "AOF 已启用 (工作目录: $BUILD_DIR)"
}

# 清空 Redis 数据
flush_redis() {
    log_info "清空 Redis 数据..."
    redis-cli FLUSHALL > /dev/null
    log_success "数据已清空"
}

# 获取 Redis INFO Persistence 中的指定字段值
_get_info_field() {
    local field=$1
    redis-cli INFO persistence 2>/dev/null | grep "^${field}:" | cut -d: -f2 | tr -d '\r'
}

# 触发 RDB 保存 - 使用 Redis 内置的 rdb_last_bgsave_duration_sec 获取真实耗时
trigger_bgsave() {
    log_info "触发 BGSAVE..."

    # 记录当前的 bgsave 持续时间（作为基准）
    local prev_duration=$(_get_info_field "rdb_last_bgsave_duration_sec")
    [ -z "$prev_duration" ] && prev_duration="0"

    # 触发 BGSAVE
    redis-cli BGSAVE > /dev/null 2>&1

    # 等待 BGSAVE 完成，使用更短的轮询间隔
    local timeout=60
    local elapsed=0
    local check_interval=0.1  # 100ms 轮询间隔

    while [ $elapsed -lt $timeout ]; do
        sleep $check_interval

        # 检查 BGSAVE 是否还在进行
        local bgsave_in_progress=$(_get_info_field "rdb_bgsave_in_progress")
        if [ "$bgsave_in_progress" = "0" ]; then
            # BGSAVE 已完成，获取实际的持续时间
            local duration_sec=$(_get_info_field "rdb_last_bgsave_duration_sec")
            [ -z "$duration_sec" ] && duration_sec="0"

            # 如果这次的 duration 和之前不同，说明是我们这次触发的
            if [ "$duration_sec" != "$prev_duration" ] && [ "$duration_sec" != "0" ]; then
                # 转换为毫秒
                local save_time_ms=$(echo "$duration_sec * 1000" | bc 2>/dev/null || echo "$(( ${duration_sec%.*} * 1000 ))")
                log_success "BGSAVE 完成 (实际耗时: ${save_time_ms}ms)"
                echo "$save_time_ms"
                return 0
            fi

            # 如果 duration 没变，可能是上次的缓存值，继续等待或确认完成
            # 通过检查 last_save_time 变化来确认
            local last_save=$(_get_info_field "rdb_last_save_time")
            if [ -n "$last_save" ] && [ "$last_save" != "0" ]; then
                # 使用 duration_sec 或至少给一个合理的估计
                local save_time_ms=$(echo "$duration_sec * 1000" | bc 2>/dev/null || echo "100")
                [ "$save_time_ms" = "0" ] && save_time_ms="100"  # 最小 100ms
                log_success "BGSAVE 完成 (实际耗时: ${save_time_ms}ms)"
                echo "$save_time_ms"
                return 0
            fi
        fi

        # 累加时间（使用整数运算避免浮点问题）
        elapsed=$((elapsed + 1))
        # 当超过 5 秒后，增加轮询间隔到 1 秒
        if [ $elapsed -gt 50 ]; then
            check_interval=1
        fi
    done

    log_error "BGSAVE 超时"
    return 1
}

# 重启 Redis - 使用高精度时间测量
restart_redis() {
    local extra_args=$1
    log_info "重启 Redis..."

    # 关闭 Redis
    redis-cli SHUTDOWN > /dev/null 2>&1 || true
    sleep 1  # 等待进程完全退出

    # 记录启动时间（纳秒级）
    local start_time=$(date +%s%N)

    # 启动 Redis，使用 build 目录作为工作目录
    if [ -n "$extra_args" ]; then
        redis-server --daemonize yes --dir "$BUILD_DIR" $extra_args
    else
        redis-server --daemonize yes --dir "$BUILD_DIR"
    fi

    # 等待启动和加载完成 - 使用 50ms 轮询间隔实现高精度测量
    local timeout=120000  # 120 秒（以毫秒为单位）
    local elapsed=0
    local check_interval=0.05  # 50ms

    while [ $elapsed -lt $timeout ]; do
        local ping_result
        ping_result=$(redis-cli ping 2>&1)
        if [ "$ping_result" = "PONG" ]; then
            local end_time=$(date +%s%N)
            local load_time_ms=$(( (end_time - start_time) / 1000000 ))
            log_success "Redis 已重启 (实际加载耗时: ${load_time_ms}ms)"
            echo "$load_time_ms"
            return 0
        fi
        # LOADING 状态也是正常的，继续等待
        sleep $check_interval
        elapsed=$((elapsed + 50))
    done

    log_error "Redis 启动失败"
    return 1
}

# 获取 RDB 文件路径
get_rdb_file() {
    local dir=$(redis-cli CONFIG GET dir | tail -1)
    echo "${dir}/dump.rdb"
}

# 获取 AOF 文件路径
get_aof_file() {
    local dir=$(redis-cli CONFIG GET dir | tail -1)
    # Redis 7.0+ 使用多部分 AOF 格式，文件在 appendonlydir 目录中
    if [ -d "${dir}/appendonlydir" ]; then
        # 找到最新的 .incr.aof 文件
        ls -t ${dir}/appendonlydir/*.incr.aof 2>/dev/null | head -1
    else
        echo "${dir}/appendonly.aof"
    fi
}

# 检查文件是否存在并返回大小
check_file() {
    local file=$1
    if [ -f "$file" ]; then
        local size
        if command -v stat > /dev/null 2>&1; then
            # Linux
            size=$(stat -c%s "$file" 2>/dev/null || echo "0")
        else
            # macOS
            size=$(stat -f%z "$file" 2>/dev/null || echo "0")
        fi
        echo "$size"
        return 0
    fi
    echo "0"
    return 1
}
