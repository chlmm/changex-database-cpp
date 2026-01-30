#!/bin/bash
#
# Redis 测试报告生成器 - 主入口脚本
# 调用各子模块完成测试和报告生成
#

set -e

# 路径设置
TEST_DIR="$(cd "$(dirname "$0")" && pwd)"
SCRIPTS_DIR="${TEST_DIR}/scripts"
BUILD_DIR="${TEST_DIR}/../build"
REPORT_DIR="${TEST_DIR}/reports"

mkdir -p "${REPORT_DIR}"

# 加载公共库
source "${SCRIPTS_DIR}/lib/common.sh"

# 测试结果汇总
RDB_RESULT="未测试"
AOF_RESULT="未测试"

# 显示主菜单
show_main_menu() {
    echo ""
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}    Redis 测试套件${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
    echo "  1) 运行性能基准测试"
    echo "  2) 运行持久化测试 (RDB/AOF)"
    echo "  3) 运行全部测试"
    echo "  0) 退出"
    echo ""
    echo -e "${BLUE}========================================${NC}"
    read -p "请选择 [0-3]: " main_choice
    
    case $main_choice in
        1) run_benchmark_tests ;;
        2) run_persistence_tests ;;
        3) run_all_tests ;;
        0) exit 0 ;;
        *) 
            log_warn "无效选择"
            exit 1
            ;;
    esac
}

# 运行性能基准测试
run_benchmark_tests() {
    log_info "运行性能基准测试..."
    
    # 检查测试程序是否存在
    if [ ! -f "${BUILD_DIR}/tests/tst_stringbenchmark" ]; then
        log_error "测试程序未找到，请先编译: cd build && cmake .. && make"
        exit 1
    fi
    
    # 运行测试
    "${SCRIPTS_DIR}/run_benchmark.sh"
    
    # 生成报告
    "${SCRIPTS_DIR}/generate_markdown_report.sh"
}

# 运行持久化测试
run_persistence_tests() {
    log_info "运行持久化测试..."
    
    # 检查测试程序是否存在
    if [ ! -f "${BUILD_DIR}/tests/tst_rdbpersistence" ]; then
        log_error "持久化测试程序未找到，请先编译"
        exit 1
    fi
    
    # 运行持久化测试
    if "${SCRIPTS_DIR}/run_all_persistence.sh"; then
        RDB_RESULT="通过 ✓"
        AOF_RESULT="通过 ✓"
    else
        RDB_RESULT="失败 ✗"
        AOF_RESULT="失败 ✗"
    fi
    
    # 生成报告
    "${SCRIPTS_DIR}/generate_markdown_report.sh" --with-persistence
}

# 运行全部测试
run_all_tests() {
    log_info "运行全部测试..."
    
    # 运行基准测试
    run_benchmark_tests
    
    # 运行持久化测试
    log_info "继续运行持久化测试..."
    if "${SCRIPTS_DIR}/run_all_persistence.sh"; then
        RDB_RESULT="通过 ✓"
        AOF_RESULT="通过 ✓"
    else
        RDB_RESULT="失败 ✗"
        AOF_RESULT="失败 ✗"
    fi
    
    # 生成完整报告
    "${SCRIPTS_DIR}/generate_markdown_report.sh" --with-persistence
}

# 主入口
check_redis || exit 1
show_main_menu

log_success "测试完成！"
