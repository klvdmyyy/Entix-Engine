add_library(EntixRuntime SHARED)

target_compile_definitions(EntixRuntime PRIVATE ERUNTIME_BUILD_DLL)

target_include_directories(EntixRuntime PUBLIC
  Source/)

target_link_libraries(EntixRuntime PUBLIC glm)
target_link_libraries(EntixRuntime PRIVATE
  SDL3::SDL3-shared
  Vendor::GLAD
  Vendor::ImGui
  Tracy::TracyClient
  nlohmann_json::nlohmann_json)

file(
  GLOB_RECURSE
  ENTIX_RUNTIME_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

target_sources(EntixRuntime PRIVATE ${ENTIX_RUNTIME_SOURCES})

add_library(Entix::Runtime ALIAS EntixRuntime)
