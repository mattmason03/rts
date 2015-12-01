set(PROJECT_NAME render)

file(GLOB TEST test/*.cpp)
file(GLOB SOURCE src/*.cpp)
file(GLOB HEADERS include/${PROJECT_NAME}/*.h)

add_library(${PROJECT_NAME} ${SOURCE} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC include ../include)
target_link_libraries(
	${PROJECT_NAME}
	#${GLFW_LIBRARIES}
	#${GLEW}
	utilities
	)

set(TEST_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_NAME} ${TEST})
target_link_libraries(${TEST_NAME} ${GTEST_BOTH_LIBRARIES})

set_property(TARGET ${TEST_NAME} PROPERTY FOLDER "test")