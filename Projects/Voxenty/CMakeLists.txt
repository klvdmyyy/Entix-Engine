add_executable(Voxenty
  Source/Main.cpp)

target_link_libraries(Voxenty PRIVATE
  Entix::Runtime
  Entix::GameFramework)
