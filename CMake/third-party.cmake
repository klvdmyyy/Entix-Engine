macro(target_link_sdl3 TARGET)
    if(MSVC)
        message(FATAL "SDL3 linking not implemented for windows. See `CMake/third-party.cmake`")
        # target_include_directories(${TARGET} PRIVATE
        #     $ENV{VULKAN_SDK}/Include)

        # target_link_directories(${TARGET} PRIVATE
        #     $ENV{VULKAN_SDK}/Lib)
        
        # target_link_libraries(${TARGET} PRIVATE
        #     vulkan-1.lib
        #     slang.lib
        #     slang-compiler.lib)
    else()
        find_package(SDL3 REQUIRED)

        target_link_libraries(${TARGET} PRIVATE
            SDL3::SDL3)
    endif()
endmacro()

macro(target_link_vulkan TARGET)
    if(MSVC)
        target_include_directories(${TARGET} PRIVATE
            $ENV{VULKAN_SDK}/Include)

        target_link_directories(${TARGET} PRIVATE
            $ENV{VULKAN_SDK}/Lib)
        
        target_link_libraries(${TARGET} PRIVATE
            vulkan-1.lib)
    else()
        find_package(Vulkan REQUIRED)

        target_link_libraries(${TARGET} PRIVATE
            Vulkan::Vulkan)
    endif()
endmacro()

macro(target_link_slang TARGET)
    if(MSVC)
        message(FATAL "Slang linking not implemented for windows. See `CMake/third-party.cmake`")
    else()
        find_package(slang REQUIRED)

        target_link_libraries(${TARGET} PRIVATE slang::slang)
    endif()
endmacro()

#####################################################
################# Fetched Libraries #################

include(FetchContent)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG v3.16.0
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(EnTT)

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
  GIT_TAG bf21d8ea115318e32ea0d97d2b4fbcb9f5773774
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
