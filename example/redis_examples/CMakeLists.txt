# Redis Examples - 分层架构图片管理示例
project(redis_examples VERSION 1.0.0)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

# Source files
set(SOURCES
    main.cpp
    models/image_model.cpp
    repositories/image_repository.cpp
    scenarios/image_scenarios.cpp
)

# Header files (for IDE support)
set(HEADERS
    models/image_model.h
    repositories/image_repository.h
    scenarios/image_scenarios.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
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

# ============ 复制第三方库到构建目录 ============
set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty)
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Copy hiredis libraries if missing
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -DOUTPUT_DIR=${OUTPUT_DIR} -DSRC_DIR=${THIRD_PARTY_DIR}/hiredis/lib -P ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CopyIfMissing.cmake
    COMMENT "Checking and copying hiredis libraries to ${OUTPUT_DIR}"
)

# Copy redis-plus-plus libraries if missing
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DOUTPUT_DIR=${OUTPUT_DIR} -DSRC_DIR=${THIRD_PARTY_DIR}/redis-plus-plus/lib -P ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CopyIfMissing.cmake
    COMMENT "Checking and copying redis-plus-plus libraries to ${OUTPUT_DIR}"
)

message(STATUS "Redis Examples: Will copy 3rdParty libraries to build directory if missing")
