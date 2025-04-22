set(CMAKE_CXX_STANDARD 20)

project(core)

set(CORE_INCLUDES "${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE ALL_SOURCES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${ALL_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDES})

set(BOOST_MIN_VERSION 1.87.0)
# use cmake -DBOOST_ROOT="path" if cmake cannot find your boost version
# TODO: make it install boost in packages folder if not found and set Boost_INCLUDE_DIR to that one
find_package(Boost REQUIRED)
if(Boost_FOUND)
    #TODO: if not found in default folder search in packages and download if necessary
    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::headers)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    message("successfully included boost ${Boost_INCLUDE_DIRS}")
endif()
