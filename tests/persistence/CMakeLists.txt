# 持久化测试

# 源文件
set(FIXTURE_SOURCES
    ${CMAKE_SOURCE_DIR}/tests/fixtures/redistestfixture.cpp
)

# RDB 持久化测试
add_executable(tst_rdbpersistence
    tst_rdbpersistence.cpp
    ${FIXTURE_SOURCES}
)
target_link_libraries(tst_rdbpersistence
    Qt5::Test
    RedisModule
)
target_include_directories(tst_rdbpersistence PRIVATE
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/tests/fixtures
)
set_target_properties(tst_rdbpersistence PROPERTIES
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# AOF 持久化测试
add_executable(tst_aofpersistence
    tst_aofpersistence.cpp
    ${FIXTURE_SOURCES}
)
target_link_libraries(tst_aofpersistence
    Qt5::Test
    RedisModule
)
target_include_directories(tst_aofpersistence PRIVATE
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/tests/fixtures
)
set_target_properties(tst_aofpersistence PROPERTIES
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)