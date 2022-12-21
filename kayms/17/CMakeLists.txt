
include_directories(${CMAKE_SOURCE_DIR}/utils)
link_directories(${CMAKE_SOURCE_DIR}/utils)

add_executable("${PROJECT_NAME}.17.1" main.c)

target_link_libraries("${PROJECT_NAME}.17.1" utils)
target_link_libraries("${PROJECT_NAME}.17.1" linkedList)