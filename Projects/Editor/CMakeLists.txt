add_executable(EntixEditor
  Source/Main.cpp
  Source/EditorLayer.cpp
  Source/Panels/PanelBase.cpp
  Source/Panels/ConsolePanel.cpp
  Source/Panels/PropertiesPanel.cpp
  Source/Panels/InspectorPanel.cpp)

target_compile_definitions(EntixEditor PRIVATE PROJECT_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Assets")

target_link_libraries(EntixEditor PRIVATE
  Entix::Runtime
  Entix::GameFramework
  Vendor::ImGui
  Vendor::ImGuizmo
  Tracy::TracyClient)
