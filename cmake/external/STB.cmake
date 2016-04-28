# Add stb
ExternalProject_Add(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    TIMEOUT 30
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Specify include dir
getLibProps(stb)
setLibVars(${source_dir})