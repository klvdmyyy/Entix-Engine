include(FetchContent)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG v3.16.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(EnTT)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.4.10
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.3
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  Tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy
  GIT_TAG v0.13.1
  OVERRIDE_FIND_PACKAGE
)

set(TRACY_ON_DEMAND OFF CACHE BOOL "" FORCE)
set(TRACY_STATIC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(Tracy)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(Runtime)
add_subdirectory(GameFramework)
add_subdirectory(Vendor)
