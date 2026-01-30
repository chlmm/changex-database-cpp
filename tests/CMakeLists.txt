# Tests Module
project(RedisTests VERSION 1.0.0)

# Find Qt Test module
find_package(Qt5 REQUIRED COMPONENTS Test)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/fixtures
    ${CMAKE_SOURCE_DIR}/RedisModule
)

# Source files for fixtures
set(FIXTURE_SOURCES
    fixtures/redistestfixture.cpp
)

# String Benchmark
add_executable(tst_stringbenchmark
    benchmarks/tst_stringbenchmark.cpp
    ${FIXTURE_SOURCES}
)
target_link_libraries(tst_stringbenchmark
    Qt5::Test
    RedisModule
)
set_target_properties(tst_stringbenchmark PROPERTIES
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Bytes Benchmark
add_executable(tst_bytesbenchmark
    benchmarks/tst_bytesbenchmark.cpp
    ${FIXTURE_SOURCES}
)
target_link_libraries(tst_bytesbenchmark
    Qt5::Test
    RedisModule
)
set_target_properties(tst_bytesbenchmark PROPERTIES
    AUTOMOC ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

# Add tests to CTest
enable_testing()
add_test(NAME StringBenchmark COMMAND tst_stringbenchmark)
add_test(NAME BytesBenchmark COMMAND tst_bytesbenchmark)

# Persistence tests
add_subdirectory(persistence)
add_test(NAME RdbPersistence COMMAND tst_rdbpersistence)
add_test(NAME AofPersistence COMMAND tst_aofpersistence)
