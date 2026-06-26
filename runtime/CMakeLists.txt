add_library(entixruntime SHARED)

target_compile_definitions(entixruntime PRIVATE ERUNTIME_BUILD_DLL TRACY_ENABLE)

target_include_directories(entixruntime PUBLIC include/)

target_include_directories(entixruntime PRIVATE source/)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Core/Base.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Core/Base.h
  @ONLY
)

include(FetchContent)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG v3.16.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.4.10
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(SDL3)

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

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(nlohmann_json)

set(TRACY_ON_DEMAND ON CACHE BOOL "" FORCE)
set(TRACY_STATIC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Tracy)

# set_target_properties(TracyClient PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(entixruntime PRIVATE
  SDL3::SDL3-shared
  glad
  Tracy::TracyClient
  nlohmann_json::nlohmann_json
  imgui)

target_link_libraries(entixruntime PUBLIC EnTT::EnTT glm::glm)

file(
  GLOB_RECURSE
  VBOX_RUNTIME_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

target_sources(entixruntime PRIVATE ${VBOX_RUNTIME_SOURCES})

add_subdirectory(benchmarks)