add_executable(Voxenty
  Source/Main.cpp
  Source/EditorLayer.cpp)

target_link_libraries(Voxenty PRIVATE
  Entix::Runtime
  Entix::GameFramework
  Vendor::ImGui)
