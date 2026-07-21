add_library(EntixGameFramework SHARED)

target_include_directories(EntixGameFramework PUBLIC Source/)

target_link_libraries(EntixGameFramework PRIVATE
  Tracy::TracyClient
  Vendor::ImGui
  Vendor::TinyObjLoader
  Vendor::STBImage)

target_link_libraries(EntixGameFramework PUBLIC
  Entix::Runtime
  EnTT::EnTT)

file(
  GLOB_RECURSE
  ENTIX_GAME_FRAMEWORK_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

target_sources(EntixGameFramework PRIVATE ${ENTIX_GAME_FRAMEWORK_SOURCES})

if(MSVC)
  target_compile_options(EntixGameFramework PRIVATE /W4 /WX)
else()
  target_compile_options(EntixGameFramework PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(Entix::GameFramework ALIAS EntixGameFramework)
