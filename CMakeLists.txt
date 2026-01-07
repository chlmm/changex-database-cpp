cmake_minimum_required(VERSION 3.14)
project(QtRedisProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 5.12.12
find_package(Qt5 REQUIRED COMPONENTS Core Network)

# ============ 3rdParty 配置开始 ============
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)
set(THIRD_PARTY_HIREDIS_DIR ${THIRD_PARTY_DIR}/hiredis)
set(THIRD_PARTY_REDISPP_DIR ${THIRD_PARTY_DIR}/redis-plus-plus)

# 添加 3rdParty 的 include 路径
include_directories(
    ${THIRD_PARTY_HIREDIS_DIR}/include
    ${THIRD_PARTY_REDISPP_DIR}/include
)

# 添加 3rdParty 的 lib 路径到链接器搜索路径
link_directories(
    ${THIRD_PARTY_HIREDIS_DIR}/lib
    ${THIRD_PARTY_REDISPP_DIR}/lib
)

message(STATUS "3rdParty libraries configured in: ${THIRD_PARTY_DIR}")
message(STATUS "If 3rdParty libraries are missing, run: ./scripts/setup_3rdparty.sh")
# ============ 3rdParty 配置结束 ============

# Add subdirectories
add_subdirectory(RedisModule)
add_subdirectory(example)
