set(project_name render)

file(GLOB test test/*.cpp)
file(GLOB source src/*.cpp)
file(GLOB headers include/${project_name}/*.h)

add_library(${project_name} ${source} ${headers})
add_dependencies(${project_name} glfw glew glm stb)
target_include_directories(${project_name} PUBLIC include ${glfw_include} ${glew_include} ${glm_include} ${stb_include})
target_link_libraries(
	${project_name}
	${OPENGL_LIBRARIES}
	${glfw_libs}
	${glew_libs}
	utilities
	)

set(test_name ${project_name}_test)
add_executable(${test_name} ${test})
add_dependencies(${test_name} gtest)
target_include_directories(${test_name} PUBLIC ${gtest_include})
target_link_libraries(${test_name} ${project_name} ${gtest_libs})
