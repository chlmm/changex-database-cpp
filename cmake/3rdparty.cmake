# cmake/3rdparty.cmake
# Configuration for 3rdParty libraries

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
