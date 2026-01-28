# RedisModule - Shared Library
project(RedisModule VERSION 1.0.0)

# Define export macro
add_definitions(-DREDISMODULE_LIBRARY)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Source files
set(MODULE_SOURCES
    redismanager.cpp
    tool/redisconnection.cpp
    tool/redisoperationsbase.cpp
    operation/redisstringoperations.cpp
    operation/redisbytesoperations.cpp
    operation/redishashoperations.cpp
    operation/redislistoperations.cpp
    operation/redissetoperations.cpp
    operation/redissortedsetoperations.cpp
    operation/redisexpirationoperations.cpp
    operation/redistransactionoperations.cpp
    operation/redisgenericoperations.cpp
)

# Header files
set(MODULE_HEADERS
    redismanager.h
    tool/redisconnection.h
    tool/redisoperationsbase.h
    operation/redisstringoperations.h
    operation/redisbytesoperations.h
    operation/redishashoperations.h
    operation/redislistoperations.h
    operation/redissetoperations.h
    operation/redissortedsetoperations.h
    operation/redisexpirationoperations.h
    operation/redistransactionoperations.h
    operation/redisgenericoperations.h
    tool/redismodule_export.h
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

install(FILES redismanager.h
    DESTINATION include/RedisModule
)
install(FILES 
    tool/redismodule_export.h
    tool/redisconnection.h
    tool/redisoperationsbase.h
    DESTINATION include/RedisModule/tool
)
install(FILES 
    operation/redisstringoperations.h
    operation/redisbytesoperations.h
    operation/redishashoperations.h
    operation/redislistoperations.h
    operation/redissetoperations.h
    operation/redissortedsetoperations.h
    operation/redisexpirationoperations.h
    operation/redistransactionoperations.h
    operation/redisgenericoperations.h
    DESTINATION include/RedisModule/operation
)
