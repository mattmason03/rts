set(project_name utilities)

file(GLOB test test/*.cpp)
file(GLOB source src/*.cpp)
file(GLOB headers include/${project_name}/*.h)

add_library(${project_name} ${source} ${headers})
target_include_directories(${project_name} PUBLIC include)
target_link_libraries(
	${project_name}
	)

set(test_name ${project_name}_test)
add_executable(${test_name} ${test})
add_dependencies(${test_name} gtest)
target_include_directories(${test_name} PUBLIC ${gtest_include} ${glfw_include})
target_link_libraries(${test_name} ${project_name} ${gtest_libs} ${glfw_libs})