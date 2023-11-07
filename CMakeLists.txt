cmake_minimum_required(VERSION 3.1)

project(ExtendedAPI VERSION 1.0)


if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  set(API_IS_MAIN TRUE)
else()
  set(API_IS_MAIN FALSE)
endif()

option(BUILD_SHARED_LIBS "Build shared lib" OFF)
option(BUILD_EXAMPLES "Build examples" ${API_IS_MAIN})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

message(STATUS "API_IS_MAIN=${API_IS_MAIN}")
message(STATUS "BUILD_EXAMPLES=${BUILD_EXAMPLES}")

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()