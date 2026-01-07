cmake_minimum_required(VERSION 3.14)
project(QtRedisProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 5.12.12
find_package(Qt5 REQUIRED COMPONENTS Core Network Gui)

# ============ 3rdParty 配置 ============
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/3rdparty.cmake)

# ============ 编译选项配置 ============
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_options.cmake)

# Add subdirectories
add_subdirectory(RedisModule)

if(BUILD_BASE_EXAMPLES)
    add_subdirectory(example/base_examples)
endif()

if(BUILD_IMAGE_MANAGEMENT)
    add_subdirectory(example/image_management)
endif()
