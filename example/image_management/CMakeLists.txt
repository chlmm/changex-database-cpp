# Image Management Example
project(image_management_example VERSION 1.0.0)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../RedisModule/include
)

# Source files
set(SOURCES
    main.cpp
    src/image_manager.cpp
    src/image_model.cpp
    demos/image_upload_demo.cpp
    demos/image_retrieve_demo.cpp
    demos/image_model_demo.cpp
    demos/image_search_demo.cpp
    demos/image_management_demo.cpp
)

# Header files
set(HEADERS
    src/image_manager.h
    src/image_model.h
    demos/image_upload_demo.h
    demos/image_retrieve_demo.h
    demos/image_model_demo.h
    demos/image_search_demo.h
    demos/image_management_demo.h
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
