# Add glew
ExternalProject_Add(
    glew
    URL http://iweb.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0-win32.zip
    TIMEOUT 30
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EP_PREFIX}/src/glew/bin/Release/x64/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/
    LOG_DOWNLOAD ON
    LOG_INSTALL ON)

# Specify include dir
ExternalProject_Get_Property(glew source_dir)
set(glew_include ${source_dir}/include)

ExternalProject_Get_Property(glew binary_dir)
set(glew_bin ${source_dir}/lib/Release/x64/glew32)
set(glew_libs ${glew_bin}.lib)
