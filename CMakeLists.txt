cmake_minimum_required(VERSION 3.4.0)
project (rts_dist)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# add dependencies
include(cmake/LoadExternalProjects.cmake)
find_package(OpenGl)

add_subdirectory(ecs ${PROJECT_BINARY_DIR}/ecs)
add_subdirectory(utilities ${PROJECT_BINARY_DIR}/utilities)
add_subdirectory(render ${PROJECT_BINARY_DIR}/render)
add_subdirectory(rts ${PROJECT_BINARY_DIR}/rts)
