set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
cmake_minimum_required(VERSION 3.31.6)

include(third-party)

project(Entix-Engine VERSION 1.0.0)

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

list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/[Bb]uild)
list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/[Oo]ut)
list(FILTER PROJECT_CL_FILES EXCLUDE REGEX ${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Vendor)

include(formatting)
clang_format(format ${PROJECT_CL_FILES})

add_subdirectory(Entix)
add_subdirectory(Sandbox)

#####################################################
############### Third Party Libraries ###############

include(FetchContent)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG v3.16.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.3
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  Tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy
  GIT_TAG v0.13.1
  OVERRIDE_FIND_PACKAGE
)

set(TRACY_ON_DEMAND OFF CACHE BOOL "" FORCE)
set(TRACY_STATIC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Tracy)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(nlohmann_json)