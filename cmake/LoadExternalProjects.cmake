# Enable ExternalProject CMake module
include(ExternalProject)

set(EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)
# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})

include(cmake/ExternalGTest.cmake)
include(cmake/ExternalGLFW.cmake)
include(cmake/ExternalGLEW.cmake)
include(cmake/ExternalGLM.cmake)
include(cmake/ExternalSTB.cmake)