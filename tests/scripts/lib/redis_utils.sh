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

# 触发 RDB 保存
trigger_bgsave() {
    log_info "触发 BGSAVE..."
    redis-cli BGSAVE > /dev/null
    
    # 等待保存完成
    local last_save_before=$(redis-cli LASTSAVE)
    local timeout=30
    local elapsed=0
    
    while [ $elapsed -lt $timeout ]; do
        sleep 1
        local last_save_after=$(redis-cli LASTSAVE)
        if [ "$last_save_after" != "$last_save_before" ]; then
            log_success "BGSAVE 完成"
            return 0
        fi
        ((elapsed++))
    done
    
    log_error "BGSAVE 超时"
    return 1
}

# 重启 Redis
restart_redis() {
    local extra_args=$1
    log_info "重启 Redis..."
    
    # 关闭 Redis
    redis-cli SHUTDOWN > /dev/null 2>&1 || true
    sleep 2
    
    # 启动 Redis，使用 build 目录作为工作目录
    if [ -n "$extra_args" ]; then
        redis-server --daemonize yes --dir "$BUILD_DIR" $extra_args
    else
        redis-server --daemonize yes --dir "$BUILD_DIR"
    fi
    
    # 等待启动和加载完成（AOF文件可能很大，需要更长时间）
    local timeout=120
    local elapsed=0
    while [ $elapsed -lt $timeout ]; do
        local ping_result
        ping_result=$(redis-cli ping 2>&1)
        if [ "$ping_result" = "PONG" ]; then
            log_success "Redis 已重启"
            return 0
        fi
        # LOADING 状态也是正常的，继续等待
        sleep 2
        ((elapsed+=2))
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
