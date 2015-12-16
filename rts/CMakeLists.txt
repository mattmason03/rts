cmake_minimum_required(VERSION 3.4.0)
set(project_name rts)
project (${project_name})

file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB_RECURSE test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

file(GLOB rts_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB components RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/Components/*.h include/Components/*.hpp)

# set vs source groups
source_group(components FILES ${components})
source_group(ecs FILES ${ecs_source})

# create executable 
add_executable(${project_name} ${sources} ${headers})
add_dependencies(${project_name} spdlog)
target_include_directories(${project_name} PUBLIC include ${spdlog_include})
# link
target_link_libraries(${project_name}
	ecs
	utilities
	render
)
add_custom_command(TARGET ${project_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
	)

enable_testing()
set(test_name ${project_name}_test)

add_executable(${test_name} ${test})
add_dependencies(${test_name} gtest)
target_include_directories(${test_name} PUBLIC ${gtest_include})
target_link_libraries(${test_name} ${gtest_libs} ecs)
