set(PROJECT_NAME ecs)

file(GLOB TEST test/*.cpp)
file(GLOB SOURCE include/ecs/*.h)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include ../include)
target_sources(${PROJECT_NAME} INTERFACE ${SOURCE})

set(TEST_NAME ${PROJECT_NAME}_test)
add_executable(${TEST_NAME} ${TEST})
target_link_libraries(${TEST_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

set_property(TARGET ${TEST_NAME} PROPERTY FOLDER "test")