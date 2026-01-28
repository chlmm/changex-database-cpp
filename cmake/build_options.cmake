# cmake/build_options.cmake
# Build options configuration

# Base Examples 编译选项（默认启用）
option(BUILD_BASE_EXAMPLES "Build base examples" ON)

# Redis Examples 编译选项（默认启用）- 新的分层架构示例
option(BUILD_REDIS_EXAMPLES "Build redis examples (layered architecture)" ON)

message(STATUS "Build base examples: ${BUILD_BASE_EXAMPLES}")
message(STATUS "Build redis examples: ${BUILD_REDIS_EXAMPLES}")
