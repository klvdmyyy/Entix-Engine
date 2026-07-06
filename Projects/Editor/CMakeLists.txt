add_executable(EntixEditor
  Source/Main.cpp
  Source/EditorLayer.cpp)

target_link_libraries(EntixEditor PRIVATE
  Entix::Runtime
  Entix::GameFramework
  Vendor::ImGui)
