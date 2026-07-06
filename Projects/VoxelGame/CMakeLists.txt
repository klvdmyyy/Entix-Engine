add_executable(VoxelGame
  Source/Main.cpp
  Source/GameLayer.cpp)

target_link_libraries(VoxelGame PRIVATE
  Entix::Runtime
  Entix::GameFramework)