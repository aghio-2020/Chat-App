cmake_minimum_required (VERSION 3.12)

set($ENV{CMAKE_BUILD_PARALLEL_LEVEL} 8)

project(project-solution)

add_subdirectory("src")