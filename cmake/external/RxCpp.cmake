# Add glm
ExternalProject_Add(
    rxcpp
    GIT_REPOSITORY https://github.com/Reactive-Extensions/RxCpp.git
    GIT_TAG v2.3.0
    TIMEOUT 30
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

# Specify include dir
getLibProps(rxcpp)
setLibVars(${source_dir}/Rx/v2/src)