cmake_minimum_required(VERSION 3.12)

project(pkg)

include(FetchContent)

set(ASIO_DWNLD_DIR ${CMAKE_CURRENT_LIST_DIR}/asio)

FetchContent_Declare(asio
  GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
  GIT_TAG         origin/master
  GIT_SHALLOW     TRUE
  SOURCE_DIR      ${ASIO_DWNLD_DIR}
)

FetchContent_GetProperties(asio)

if(NOT asio_POPULATED)
  message(STATUS "Fetching asio ${ASIO_DWNLD_DIR}")

  if(EXISTS "${ASIO_CMAKE_ASIO_DEP_DIR}/asio")
    set(FETCHCONTENT_SOURCE_DIR_ASIO ${ASIO_CMAKE_ASIO_DEP_DIR})
    message(STATUS "asio ${ASIO_DWNLD_DIR} source found at ${ASIO_CMAKE_ASIO_DEP_DIR}; skip fetching.")
  endif()

  FetchContent_Populate(asio)
  message(STATUS "Populating asio ${ASIO_DWNLD_DIR} done; src at ${asio_SOURCE_DIR}")
endif()