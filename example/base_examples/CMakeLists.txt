# Example - Demo Application
project(example VERSION 1.0.0)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../RedisModule/include
)

# Source files
set(EXAMPLE_SOURCES
    main.cpp
    demos/basic_string_demo.cpp
    demos/hash_demo.cpp
    demos/list_demo.cpp
    demos/set_demo.cpp
    demos/sorted_set_demo.cpp
    demos/expiration_demo.cpp
    demos/user_profile_example.cpp
    demos/leaderboard_example.cpp
)

# Header files
set(EXAMPLE_HEADERS
    demos/basic_string_demo.h
    demos/hash_demo.h
    demos/list_demo.h
    demos/set_demo.h
    demos/sorted_set_demo.h
    demos/expiration_demo.h
    demos/user_profile_example.h
    demos/leaderboard_example.h
)

# Create executable
add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES} ${EXAMPLE_HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Network
    RedisModule
)

# Set executable properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Set runtime library path (RPATH)
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH "$ORIGIN"
)

# Copy 3rdParty libraries to build directory for portability
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rdParty)
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Check if hiredis libraries exist, if not copy them
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -DOUTPUT_DIR=${OUTPUT_DIR} -DSRC_DIR=${THIRD_PARTY_DIR}/hiredis/lib -P ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CopyIfMissing.cmake
    COMMENT "Checking and copying hiredis libraries to ${OUTPUT_DIR}"
)

# Check if redis-plus-plus libraries exist, if not copy them
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DOUTPUT_DIR=${OUTPUT_DIR} -DSRC_DIR=${THIRD_PARTY_DIR}/redis-plus-plus/lib -P ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CopyIfMissing.cmake
    COMMENT "Checking and copying redis-plus-plus libraries to ${OUTPUT_DIR}"
)

message(STATUS "Will copy 3rdParty libraries to build directory: ${OUTPUT_DIR} (if missing)")
