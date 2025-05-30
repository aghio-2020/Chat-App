set(CMAKE_CXX_STANDARD 20)

project(core)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(PKG_DIR "${CMAKE_SOURCE_DIR}/packages")
# so find_package will look into my packages folder as well
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${PKG_DIR}/boost")

set(JSON_INCLUDES "${PKG_DIR}/json/include")
set(IMGUI_DIR "${PKG_DIR}/imgui")
set(GLFW_INCLUDES "${PKG_DIR}/glfw/include")
set(ASIO_INCLUDES "${PKG_DIR}/asio/asio/include/")

# set core library
set(CORE_INCLUDES "${PROJECT_SOURCE_DIR}/include")
file(GLOB_RECURSE CORE_SOURCES
    "${PROJECT_SOURCE_DIR}/*.h"
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${CORE_SOURCES})

### PACKAGES ###
###################################################################################################
#######################################################################################

add_subdirectory(${PKG_DIR} ${CMAKE_CURRENT_BINARY_DIR}/packages/)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
)

if(UNIX)
    target_link_libraries(${PROJECT_NAME} INTERFACE pthread)
elseif(WIN32)
    set(_WIN32_WINNT 0x0601)

    message(STATUS "Set _WIN32_WINNET=${_WIN32_WINNT}")

    add_compile_definitions(
        _WIN32_WINNT=${_WIN32_WINNT}
        WIN32_LEAN_AND_MEAN
    )
endif()

######################################################################################
###################################################################################################

file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.h"
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.h"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.h"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.cpp"
)

find_package(OpenGL REQUIRED)

add_subdirectory(${PKG_DIR}/glfw ${CMAKE_CURRENT_BINARY_DIR}/packages/glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "GLFW lib only" FORCE)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${OPENGL_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CORE_INCLUDES} ${JSON_INCLUDES} ${IMGUI_DIR} ${GLFW_INCLUDES} ${ASIO_INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Core")
set_target_properties(imgui PROPERTIES FOLDER "Core/Dependencies")
set_target_properties(glfw uninstall update_mappings PROPERTIES FOLDER "Core/Dependencies/GLFW3")

set(BOOST_MIN_VERSION 1.87.0)
# use cmake -DBOOST_ROOT="path" if cmake cannot find your boost version
# TODO: make it install boost in packages folder if not found and set Boost_INCLUDE_DIR to that one
find_package(Boost REQUIRED)

if(Boost_FOUND)
    #TODO: if not found in default folder search in packages and download if necessary
    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::headers)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
    message("successfully included boost ${Boost_INCLUDE_DIRS}")
endif()
