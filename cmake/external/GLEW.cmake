# Add glew
ExternalProject_Add(
    glew
    URL http://iweb.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0-win32.zip
    TIMEOUT 30
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_if_different ${EP_PREFIX}/src/glew/bin/Release/x64/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/
    LOG_DOWNLOAD ON
    LOG_INSTALL ON)

# Specify include dir
getLibProps(glew)
setLibVars(${source_dir}/include ${source_dir}/lib/Release/x64/glew32.lib)
