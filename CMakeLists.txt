set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
cmake_minimum_required(VERSION 3.31.6)

project(Entix-Engine VERSION 1.0.0)

include(third-party)
include(vulkan-sdk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin)

if (MINGW AND (CMAKE_CXX_COMPILER_ID STREQUAL GNU OR CMAKE_CXX_COMPILER_ID STREQUAL Clang))
  link_libraries(stdc++exp) # std:: print, println, ...
endif()

file(
  GLOB_RECURSE
  CL_FILES
  *.[hc]
  *.[hc]pp
  *.vert
  *.frag
  *.glsl
)

file(
  GLOB_RECURSE
  PROJECT_CL_FILES
  *.[hc]
  *.[hc]pp
  *.vert
  *.frag
  *.glsl
)

list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/[Bb]uild)
list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/[Oo]ut)
list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/Entix/Vendor)

include(formatting)
clang_format(format ${PROJECT_CL_FILES})

option(ENTIX_BUILD_STATIC
  "Build Entix Engine as static library."
  OFF
)

add_subdirectory(Entix)
add_subdirectory(Sandbox)
