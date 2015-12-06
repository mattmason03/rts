# Add glfw
ExternalProject_Add(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.1.2
    TIMEOUT 30
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DBUILD_SHARED_LIBS=ON
                -DGLFW_BUILD_EXAMPLES=OFF
                -DGLFW_BUILD_TESTS=OFF
                -DGLFW_BUILD_DOCS=OFF
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
                -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# Specify include dir
ExternalProject_Get_Property(glfw source_dir)
set(glfw_include ${source_dir}/include)

ExternalProject_Get_Property(glfw binary_dir)
set(glfw_bin ${binary_dir}/src/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}glfw3dll)
set(glfw_libs ${glfw_bin}.lib)
