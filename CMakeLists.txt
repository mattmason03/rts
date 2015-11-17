cmake_minimum_required(VERSION 3.4.0)
set(PROJECT_NAME rts)
project (${PROJECT_NAME})

set(CMAKE_INCLUDE_PATH include)
set(CMAKE_LIBRARY_PATH lib)

# recursively get rts sources
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB_RECURSE TESTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)
file(GLOB_RECURSE ECS_SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ecs/include/*.h)

file(GLOB RTS_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h include/*.hpp)
file(GLOB COMP_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/Components/*.h include/Components/*.hpp)

# set vs source groups
source_group(RTSSource FILES ${SOURCES})
source_group(RTSHeaders FILES ${RTS_HEADERS})
source_group(Components FILES ${COMP_HEADERS})
source_group(ECS FILES ${ECS_SOURCE})

# add dependencies

find_package(GLEW)
find_package(GTest)
find_package(OpenGl)
include(GLFWConfig.cmake)

message(STATUS "${GTEST_BOTH_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES}")

add_subdirectory(ecs)
add_subdirectory(utilities)
add_subdirectory(render)

# create executable 
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
# link
target_link_libraries(${PROJECT_NAME}
	${GLFW_LIBRARIES} 
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
	ecs
	utilities
	render
)
target_include_directories(${PROJECT_NAME} PUBLIC include)

enable_testing()
set(TEST_NAME ${PROJECT_NAME}_test)

# create test executable
add_executable(${TEST_NAME} ${TESTS})
# link
target_link_libraries(${TEST_NAME}
	${GTEST_BOTH_LIBRARIES}
	ecs
)
target_include_directories(${TEST_NAME} PUBLIC include )

set_property(TARGET ${TEST_NAME} PROPERTY FOLDER "test")
