# cmake/CopyIfMissing.cmake
# CMake script to copy library files if they don't exist

# Get the directory name from source path
get_filename_component(DIR_NAME ${SRC_DIR} NAME)

# Map directory names to library names
if(DIR_NAME STREQUAL "hiredis")
    set(LIB_NAME "hiredis")
elseif(DIR_NAME STREQUAL "redis-plus-plus")
    set(LIB_NAME "redis++")
else()
    set(LIB_NAME "${DIR_NAME}")
endif()

# Check if any of the library files exist in output directory
set(LIB_EXISTS FALSE)

# Check for common library file patterns
if(EXISTS "${OUTPUT_DIR}/lib${LIB_NAME}.so" OR
   EXISTS "${OUTPUT_DIR}/lib${LIB_NAME}.so.1" OR
   EXISTS "${OUTPUT_DIR}/lib${LIB_NAME}.a")
    set(LIB_EXISTS TRUE)
endif()

# Copy only if libraries don't exist
if(NOT LIB_EXISTS)
    message(STATUS "Copying ${LIB_NAME} libraries from ${SRC_DIR} to ${OUTPUT_DIR}")
    file(COPY ${SRC_DIR}/ DESTINATION ${OUTPUT_DIR} FILES_MATCHING PATTERN "*.so*")
else()
    message(STATUS "${LIB_NAME} libraries already exist, skipping copy")
endif()
