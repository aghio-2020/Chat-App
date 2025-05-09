set(CMAKE_CXX_STANDARD 20)

project(server)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.cpp"
)
set(CORE_INCLUDES "${PROJECT_SOURCE_DIR}/../core/include")

if(WIN32)
    set(_WIN32_WINNT 0x0601)

    message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

    add_compile_definitions(
        _WIN32_WINNT=${_WIN32_WINNT}
        WIN32_LEAN_AND_MEAN
    )
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE core)
target_include_directories(${PROJECT_NAME} PRIVATE ${CORE_INCLUDES} ${PROJECT_SOURCE_DIR})