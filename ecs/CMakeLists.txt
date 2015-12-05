set(project_name ecs)

file(GLOB test test/*.cpp)
file(GLOB ecs_source include/ecs/*.h)

source_group(ecs FILES ${ecs_source})

add_library(${project_name} INTERFACE)
target_include_directories(${project_name} INTERFACE include)
target_sources(${project_name} INTERFACE ${ecs_source})

set(test_name ${project_name}_test)
add_executable(${test_name} ${test})
add_dependencies(${test_name} gtest)
target_include_directories(${test_name} PUBLIC ${gtest_include})
target_link_libraries(${test_name} ${project_name} ${gtest_libs})

