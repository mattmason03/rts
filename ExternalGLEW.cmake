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
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON)

# Specify include dir
ExternalProject_Get_Property(glew source_dir)
set(glew_include ${source_dir}/include)

ExternalProject_Get_Property(glew binary_dir)
set(glew_bin ${source_dir}/lib/Release/x64/glew32)
set(glew_libs ${glew_bin}.lib)

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
#     debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}glew${Suffix}
#     optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}glew${Suffix}
#     ${Pthread})