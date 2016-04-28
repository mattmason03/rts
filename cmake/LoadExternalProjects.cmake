# Enable ExternalProject CMake module
include(ExternalProject)

function(getLibProps lib)
  ExternalProject_Get_Property(${lib} source_dir)
  ExternalProject_Get_Property(${lib} binary_dir)
  set(source_dir ${source_dir} PARENT_SCOPE)
  set(binary_dir ${binary_dir} PARENT_SCOPE)
  set(extLibNames ${extLibNames} ${lib} PARENT_SCOPE)
endfunction(getLibProps)

function(setLibVars incDir)
  set(libDir ${ARGV1})
  set(extInc ${extInc} ${incDir} PARENT_SCOPE)
  if(libDir)
    set(extLib ${extLib} ${libDir} PARENT_SCOPE)
  endif(libDir)
endfunction(setLibVars)

# Have external project source live in 
set(EP_PREFIX ${CMAKE_SOURCE_DIR}/ThirdParty)
# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${EP_PREFIX})

file(GLOB externalProjects ${CMAKE_CURRENT_LIST_DIR}/external/*.cmake)
foreach(externalProject ${externalProjects})
  include(${externalProject})
endforeach()