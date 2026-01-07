# cmake/build_options.cmake
# Build options configuration

# Base Examples 编译选项（默认启用）
option(BUILD_BASE_EXAMPLES "Build base examples" ON)

# Image Management Examples 编译选项（默认启用）
option(BUILD_IMAGE_MANAGEMENT "Build image management examples" ON)

message(STATUS "Build base examples: ${BUILD_BASE_EXAMPLES}")
message(STATUS "Build image management examples: ${BUILD_IMAGE_MANAGEMENT}")
