set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
cmake_minimum_required(VERSION 3.31.6)

project(Entix VERSION 1.0.0)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# set(CMAKE_CXX_STANDARD 26)
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

include(etags)
etags_generate(${CL_FILES})

file(
  GLOB_RECURSE
  PROJECT_CL_FILES
  *.[hc]
  *.[hc]pp
  *.vert
  *.frag
  *.glsl
)

list(FILTER VBOX_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
list(FILTER VBOX_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/[Bb]uild)
list(FILTER VBOX_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/[Oo]ut)
list(FILTER VBOX_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Vendor)

include(formatting)
clang_format(format ${PROJECT_CL_FILES})

set(ENTIX_SHADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)

add_subdirectory(Libraries)
add_subdirectory(Projects)
