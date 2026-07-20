add_executable(EntixEditor
  Source/Main.cpp
  Source/EditorLayer.cpp)

target_compile_definitions(EntixEditor PRIVATE PROJECT_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets")

target_link_libraries(EntixEditor PRIVATE
  Entix::Runtime
  Entix::GameFramework
  Vendor::ImGui
  Tracy::TracyClient)
