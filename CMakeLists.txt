cmake_minimum_required(VERSION 3.4.0)
project (rts_dist)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# add dependencies
include(cmake/LoadExternalProjects.cmake)

add_subdirectory(rts ${PROJECT_BINARY_DIR}/rts)
