add_subdirectory(Vendor)  # Vendor Libraries
add_library(Entix SHARED) # Entix-Engine

if(MSVC)
    set_target_properties(Entix PROPERTIES
        VS_GLOBAL_RunCodeAnalysis true

        # Use visual studio core guidelines
        VS_GLOBAL_EnableMicrosoftCodeAnalysis true

        # Use clangtidy
        VS_GLOBAL_EnableClangTidyCodeAnalysis true
        VS_GLOBAL_ClangTidyChecks -checks=*,modernize-*,-modernize-use-trailing-return-type
    )
endif()

#####################################################
############### Precompiled Headers #################

target_precompile_headers(Entix PRIVATE
    <iostream>
    <memory>
    <utility>
    <algorithm>
    <functional>
    <string>
    <sstream>
    <vector>
    <array>
    <unordered_map>
    <unordered_set>
    <span>
    <variant>
    <cstddef>
    <cstdint>
    <print>
    <filesystem>)

#####################################################
########### Compiler Options/Definitions ############

target_compile_definitions(Entix PRIVATE
    ENTIX_BUILD_DLL
    $<$<CONFIG:Debug>:ENTIX_BUILD_DEBUG>)

if(MSVC)
    target_compile_options(Entix PRIVATE
        /utf-8
        /W4 /WX)
    
    target_compile_options(Entix PUBLIC
        $<$<CONFIG:Debug>:/fsanitize=address>)
else()
    target_compile_options(Entix PRIVATE
        -fexec-charset=UTF-8
        -finput-charset=UTF-8
        -Wall -Wextra -Wpedantic -Werror)
    
    target_compile_options(Entix PUBLIC
        $<$<CONFIG:Debug>:-fsanitize=address>)
endif()

#####################################################
################# Configured Files ##################

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/Core/Base.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/Core/Base.h
    @ONLY
)

#####################################################
################ Include Directories ################

target_include_directories(Entix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source)

#####################################################
############## Third-Party Libraries ################

if(MSVC)
    target_include_directories(Entix PRIVATE
        ${VULKAN_INCLUDE_DIRECTORY})

    target_link_directories(Entix PRIVATE
        ${VULKAN_LINK_DIRECTORY})
    
    target_link_libraries(Entix PRIVATE
        vulkan-1.lib)
endif()

target_link_libraries(Entix PRIVATE
    Vendor::GLAD
    Vendor::TinyObjLoader
    Vendor::STBImage
    Vendor::ImGui)

#####################################################
################# Setup Source Files ################

file(
    GLOB_RECURSE
    ENTIX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/*.cpp
)

file(
    GLOB_RECURSE
    PLATFORM_SDL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/SDL/*.cpp
)

file(
    GLOB_RECURSE
    PLATFORM_OPENGL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/OpenGL/*.cpp
)

file(
    GLOB_RECURSE
    PLATFORM_VULKAN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Vulkan/*.cpp
)

file(
    GLOB_RECURSE
    PLATFORM_DIRECT3D_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/Direct3D/*.cpp
)

target_sources(Entix PRIVATE ${ENTIX_SOURCES})

if(WIN32)
    target_sources(Entix PRIVATE
        ${PLATFORM_SDL_SOURCES}
        ${PLATFORM_OPENGL_SOURCES}
        ${PLATFORM_VULKAN_SOURCES}
        ${PLATFORM_DIRECT3D_SOURCES})
elseif(LINUX)
    target_sources(Entix PRIVATE
        ${PLATFORM_SDL_SOURCES}
        ${PLATFORM_OPENGL_SOURCES}
        ${PLATFORM_VULKAN_SOURCES})
else()
    message(FATAL_ERROR "Platform doesn't support current platform!")
endif()