cmake_minimum_required(VERSION 3.4.0)
set(project_name rts)
project (${project_name})

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB_RECURSE test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

file(GLOB rts_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB components RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/Components/*.h include/Components/*.hpp)

# set vs source groups
source_group(source FILES ${sources})
source_group(headers FILES ${rts_headers})
source_group(components FILES ${components})

# create executable 
add_executable(${project_name} ${sources} ${headers})
target_include_directories(${project_name} PUBLIC include)
# link
target_link_libraries(${project_name}
	ecs
	utilities
	render
)

enable_testing()
set(test_name ${project_name}_test)

add_executable(${test_name} ${test})
add_dependencies(${test_name} gtest)
target_include_directories(${test_name} PUBLIC ${gtest_include})
target_link_libraries(${test_name} ${gtest_libs} ecs)
