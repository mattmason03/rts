# Enable ExternalProject CMake module
include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

include(cmake/ExternalGTest.cmake)
include(cmake/ExternalGLFW.cmake)
include(cmake/ExternalGLEW.cmake)
include(cmake/ExternalGLM.cmake)
include(cmake/ExternalSTB.cmake)