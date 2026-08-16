add_subdirectory(Vendor)  # Vendor Libraries

# Helper functions

function(collect_module_files MODULE_NAME)
    set(HEADERS)
    set(SOURCES)

    file(GLOB_RECURSE MODULE_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/${MODULE_NAME}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/${MODULE_NAME}/*.hpp"
    )
    list(APPEND HEADERS ${MODULE_HEADERS})
    
    file(GLOB_RECURSE MODULE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/${MODULE_NAME}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/${MODULE_NAME}/*.cpp"
    )
    list(APPEND SOURCES ${MODULE_SOURCES})

    set(ENTIX_HEADERS ${ENTIX_HEADERS} ${HEADERS} PARENT_SCOPE)
    set(ENTIX_SOURCES ${ENTIX_SOURCES} ${SOURCES} PARENT_SCOPE)
endfunction()

function(collect_platform_files PLATFORM_NAME)
    set(HEADERS)
    set(SOURCES)

    file(GLOB_RECURSE PLATFORM_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/${PLATFORM_NAME}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/${PLATFORM_NAME}/*.hpp"
    )
    list(APPEND HEADERS ${PLATFORM_HEADERS})
    
    file(GLOB_RECURSE PLATFORM_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/${PLATFORM_NAME}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source/Platform/${PLATFORM_NAME}/*.cpp"
    )
    list(APPEND SOURCES ${PLATFORM_SOURCES})

    set(ENTIX_HEADERS ${ENTIX_HEADERS} ${HEADERS} PARENT_SCOPE)
    set(ENTIX_SOURCES ${ENTIX_SOURCES} ${SOURCES} PARENT_SCOPE)
endfunction()

# Module files

collect_module_files(Core)
collect_module_files(Resources)
collect_module_files(RHI)
collect_module_files(Serialization)
collect_module_files(Utils)
collect_module_files(WSI)

collect_module_files(Game)

# Platform files

collect_platform_files(SDL)

collect_platform_files(Vulkan)

if(WIN32)
    collect_platform_files(Direct3D)
endif()

# Library definition

if(ENTIX_BUILD_STATIC)
    add_library(Entix STATIC
        ${ENTIX_HEADERS} ${ENTIX_SOURCES})
else()
    add_library(Entix SHARED
        ${ENTIX_HEADERS} ${ENTIX_SOURCES})
endif()

target_include_directories(Entix PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# ClangTidy, VisualStudio code analysis

include(utilities)

force_utf8_for(Entix)
enable_code_analysis_for(Entix)
enable_all_warnings_for(Entix)
enable_sanitizers_for(Entix)
warnings_are_errors_for(Entix)

# Configured files

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/Core/Base.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Entix/Core/Base.h
    @ONLY
)

# Third-Party libraries

include(third-party)

target_link_libraries(Entix PRIVATE
    Vendor::GLAD
    Vendor::TinyObjLoader
    Vendor::STBImage
    Vendor::ImGui
    VulkanSDK::Headers
    VulkanSDK::Slang
    VulkanSDK::SlangCompiler
    ThirdParty::SDL3
    ThirdParty::TracyClient)

target_link_libraries(Entix PUBLIC
    ThirdParty::CLI11
    ThirdParty::JSON)

# Compiler definitions

target_compile_definitions(Entix PRIVATE
    ENTIX_BUILD_DLL)

target_compile_definitions(Entix PUBLIC
    $<$<CONFIG:Debug>:ENTIX_BUILD_DEBUG>)

# Precompiled headers

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
    <filesystem>
    <any>
    <condition_variable>
    <queue>
    <mutex>
    <thread>
    <type_traits>
    <future>)
