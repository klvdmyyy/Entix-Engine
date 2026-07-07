add_executable(VoxelGame
  Source/Main.cpp
  Source/GameLayer.cpp)

file(
  GLOB_RECURSE
  VOXEL_GAME_SCRIPTS
  ${CMAKE_CURRENT_SOURCE_DIR}/Source/Scripts/*.cpp
)

target_sources(VoxelGame PRIVATE ${VOXEL_GAME_SCRIPTS})

target_link_libraries(VoxelGame PRIVATE
  Entix::Runtime
  Entix::GameFramework)