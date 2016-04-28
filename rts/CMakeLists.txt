cmake_minimum_required(VERSION 3.4.0)
set(projectName rts)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project (${projectName})

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
list(REMOVE_ITEM sources src/main.cpp)
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB_RECURSE test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

# set vs source groups
function(addSourceGroup sourceName isInclude)
  set(dir src)
  if(isInclude)
    set(dir include)
  endif(isInclude)
  
  file(GLOB sourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${dir}/${sourceName}/*)
  source_group(${sourceName}_${dir} FILES ${sourceFiles})
endfunction(addSourceGroup)

addSourceGroup(Components 1)
addSourceGroup(ecs 1)
addSourceGroup(render 1)
addSourceGroup(render 0)
addSourceGroup(utilities 1)
addSourceGroup(utilities 0)

set(libName ${projectName}_lib)
add_library(${libName} STATIC ${sources} ${headers})
add_dependencies(${libName} ${extLibNames})
target_include_directories(${libName} PUBLIC include ${extInc})
target_link_libraries(${libName} ${extLib})

# create executable 
add_executable(${projectName} src/main.cpp)

# ensure that external projects are built first
add_dependencies(${projectName} ${libName})

# add include directories, local and external
target_include_directories(${projectName} PUBLIC include ${extInc})

# link with external libs
target_link_libraries(${projectName} ${extLib} ${libName})

# copy resources
add_custom_command(TARGET ${projectName} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
	)

# setup test project
# TODO: make most code lib to link against exe and test proj
enable_testing()
set(testName ${projectName}_test)

add_executable(${testName} ${test})
add_dependencies(${testName} ${libName} gtest)
target_include_directories(${testName} PUBLIC ${gtest_include} include ${extInc})
target_link_libraries(${testName} ${gtest_libs} ${extLib} ${libName})