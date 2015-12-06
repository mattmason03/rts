# Add gtest
ExternalProject_Add(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.7.0
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
ExternalProject_Get_Property(gtest source_dir)
message(STATUS ${source_dir})
set(gtest_include ${source_dir}/include)

ExternalProject_Get_Property(gtest binary_dir)
set(gtest_bin ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest)
set(gtest_libs ${gtest_bin}.lib ${gtest_bin}_main.lib)
