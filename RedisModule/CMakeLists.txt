# RedisModule - Shared Library
project(RedisModule VERSION 1.0.0)

# Define export macro
add_definitions(-DREDISMODULE_LIBRARY)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/RedisModule
)

# Source files
set(MODULE_SOURCES
    src/redismanager.cpp
    src/redisconnection.cpp
    src/redisstringoperations.cpp
    src/redishashoperations.cpp
    src/redislistoperations.cpp
    src/redissetoperations.cpp
    src/redissortedsetoperations.cpp
    src/redisexpirationoperations.cpp
    src/redistransactionoperations.cpp
    src/redisgenericoperations.cpp
)

# Header files
set(MODULE_HEADERS
    include/redismanager.h
    include/redisconnection.h
    include/redisstringoperations.h
    include/redishashoperations.h
    include/redislistoperations.h
    include/redissetoperations.h
    include/redissortedsetoperations.h
    include/redisexpirationoperations.h
    include/redistransactionoperations.h
    include/redisgenericoperations.h
    include/redismodule_export.h
)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${MODULE_SOURCES} ${MODULE_HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Network
    redis++
    hiredis
)

# Set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Set runtime library path (RPATH)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN/../../3rdParty/hiredis/lib:$ORIGIN/../../3rdParty/redis-plus-plus/lib"
    BUILD_RPATH "$ORIGIN/../../3rdParty/hiredis/lib:$ORIGIN/../../3rdParty/redis-plus-plus/lib"
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES ${MODULE_HEADERS}
    DESTINATION include/RedisModule
)
