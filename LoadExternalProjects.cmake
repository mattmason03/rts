# Enable ExternalProject CMake module
include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

include(ExternalGTest.cmake)
include(ExternalGLFW.cmake)
include(ExternalGLEW.cmake)
include(ExternalGLM.cmake)