# ---------------------------------------------------------------------
# GLAD
# ---------------------------------------------------------------------
add_library(glad glad/src/glad.c)

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(glad PUBLIC glad/include)

add_library(Vendor::GLAD ALIAS glad)

# ---------------------------------------------------------------------
# ImGUI (Docking Branch)
# ---------------------------------------------------------------------
add_library(imgui)

set_target_properties(imgui PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(imgui PUBLIC
  imgui-docking
  imgui-docking/misc/cpp
  imgui-docking/backends)

file(
  GLOB
  IMGUI_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/misc/cpp/imgui_stdlib.cpp
)

file(
  GLOB
  IMGUI_SDL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/backends/imgui_impl_sdl3.cpp
)

file(
  GLOB
  IMGUI_OPENGL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/backends/imgui_impl_opengl3.cpp
)

target_sources(imgui PRIVATE
  ${IMGUI_SOURCES}
  ${IMGUI_SDL_SOURCES}
  ${IMGUI_OPENGL_SOURCES})

add_library(Vendor::ImGui ALIAS imgui)
