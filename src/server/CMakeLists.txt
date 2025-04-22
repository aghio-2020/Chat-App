set(CMAKE_CXX_STANDARD 20)

project(server)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.cpp"
)
set(CORE_INCLUDES "${PROJECT_SOURCE_DIR}/../core/include")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CORE_INCLUDES} ${PROJECT_SOURCE_DIR})