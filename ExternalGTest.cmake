# Enable ExternalProject CMake module
include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

# Add gtest
ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    TIMEOUT 30
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -Dgtest_force_shared_crt=ON
              -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
set(gtest_include ${source_dir}/include)

ExternalProject_Get_Property(googletest binary_dir)
set(gtest_bin ${binary_dir}/googlemock/gtest/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest)
set(gtest_libs ${gtest_bin}.lib ${gtest_bin}_main.lib)

# # Add test executable target
# add_executable(MainTest ${PROJECT_SOURCE_DIR}/src/main.cpp)

# # Create dependency of MainTest on googletest
# add_dependencies(MainTest googletest)

# # Specify MainTest's link libraries
# ExternalProject_Get_Property(googletest binary_dir)
# if(MSVC)
#   set(Suffix ".lib")
# else()
#   set(Suffix ".a")
#   set(Pthread "-pthread")
# endif()
# target_link_libraries(
#     MainTest
#     debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}
#     optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}
#     ${Pthread})