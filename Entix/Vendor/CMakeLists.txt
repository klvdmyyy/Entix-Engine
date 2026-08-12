# ---------------------------------------------------------------------
# GLAD
# ---------------------------------------------------------------------
add_library(glad glad/src/glad.c)

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(glad PUBLIC glad/include)

add_library(Vendor::GLAD ALIAS glad)

# ---------------------------------------------------------------------
# ImGui (Docking Branch)
# ---------------------------------------------------------------------
add_library(imgui_headers__entix_vendor INTERFACE)

target_include_directories(imgui_headers__entix_vendor INTERFACE
  imgui-docking
  imgui-docking/misc/cpp
  imgui-docking/backends)

add_library(imgui__entix_vendor)

set_target_properties(imgui__entix_vendor PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(imgui__entix_vendor PUBLIC imgui-headers)

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

file(
  GLOB
  IMGUI_VULKAN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/backends/imgui_impl_vulkan.cpp
)

target_link_vulkan(imgui__entix_vendor)
target_link_sdl3(imgui__entix_vendor)

target_sources(imgui__entix_vendor PRIVATE
  ${IMGUI_SOURCES}
  ${IMGUI_SDL_SOURCES}
  ${IMGUI_OPENGL_SOURCES}
  ${IMGUI_VULKAN_SOURCES})

add_library(Vendor::ImGui-Headers ALIAS imgui_headers__entix_vendor)
add_library(Vendor::ImGui ALIAS imgui__entix_vendor)

# ---------------------------------------------------------------------
# ImGuizmo (Docking Branch)
# ---------------------------------------------------------------------
# add_subdirectory(ImGuizmo-master)
add_library(imguizmo)

target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo-master/src)

target_link_libraries(imguizmo PRIVATE
  Vendor::ImGui)

file(
  GLOB_RECURSE
  IMGUIZMO_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo-master/src/*.cpp
)

target_sources(imguizmo PRIVATE
  ${IMGUIZMO_SOURCES})

add_library(Vendor::ImGuizmo ALIAS imguizmo)

# ---------------------------------------------------------------------
# Tiny Obj Loader
# ---------------------------------------------------------------------
add_library(tinyobjloader INTERFACE)

target_include_directories(tinyobjloader INTERFACE tinyobjloader/)

add_library(Vendor::TinyObjLoader ALIAS tinyobjloader)

# ---------------------------------------------------------------------
# STB Image
# ---------------------------------------------------------------------
add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE stb_image/)

add_library(Vendor::STBImage ALIAS stb_image)
