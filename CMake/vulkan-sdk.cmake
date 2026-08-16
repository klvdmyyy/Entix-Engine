#----------------------------------------+
# Copyright (C) 2026 Dmitriy Klementiev. |
#                                        |
# SPDX-License-Identifier: BSD-3-Clause  |
#----------------------------------------+---------------------------------------
# VulkanSDK automatic installation via CMake.
#
# > On linux it just installed VulkanSDK archive and unpack it into cmake binary
# dir.
#
# > On windows it download installer and install VulkanSDK for your system.
#--------------------------------------------------------------------------------

# Check current platform
if(NOT (MSVC OR LINUX))
    message(FATAL_ERROR "Vulkan SDK installation doesn't supported on current platform.")
endif()

# Setting up Vulkan SDK version
set(VULKAN_SDK_VERSION "1.4.350.0" CACHE STRING "Vulakn SDK version")

if(MSVC)
    set(VULKAN_SDK_PLATFORM_NAME "windows")
    set(VULKAN_SDK_ARCH "X64")
    set(VULKAN_SDK_EXTENSION "exe")
elseif(LINUX)
    set(VULKAN_SDK_PLATFORM_NAME "linux")
    set(VULKAN_SDK_ARCH "x86_64")
    set(VULKAN_SDK_EXTENSION "tar.xz")
endif()

set(VULKAN_SDK_URL "https://sdk.lunarg.com/sdk/download/${VULKAN_SDK_VERSION}/${VULKAN_SDK_PLATFORM_NAME}/vulkansdk-${VULKAN_SDK_PLATFORM_NAME}-${VULKAN_SDK_ARCH}-${VULKAN_SDK_VERSION}.${VULKAN_SDK_EXTENSION}")

if(MSVC)
    # Installation of Vulkan SDK on Windows is a little bit different.
    #
    # We need to run EXE installer. But before we do this, we want to check
    # if VulkanSDK already installed and it has right version.
    if(DEFINED ENV{VULKAN_SDK})
        set(INSTALLED_VULKAN_SDK_PATH $ENV{VULKAN_SDK})
        cmake_path(GET INSTALLED_VULKAN_SDK_PATH FILENAME INSTALLED_VULKAN_SDK_VERSION_DIRECTORY)
        if(NOT "${VULKAN_SDK_VERSION}" STREQUAL "${INSTALLED_VULKAN_SDK_VERSION_DIRECTORY}")
            message(STATUS "Vulkan SDK version mismatch. Installed version is ${INSTALLED_VULKAN_SDK_VERSION_DIRECTORY}. Required version is ${VULKAN_SDK_VERSION}.")
            message(STATUS "Installing Vulkan SDK ${VULKAN_SDK_VERSION}")

            # TODO VulkanSDK installation on Windows
        endif()
    endif()
elseif(LINUX)
    FetchContent_Declare(VulkanSDK
        URL ${VULKAN_SDK_URL}
    )

    FetchContent_MakeAvailable(VulkanSDK)
    FetchContent_GetProperties(VulkanSDK
        SOURCE_DIR VulkanSDK_SOURCE_DIR
    )

    set(VULKAN_SDK_ROOT ${VulkanSDK_SOURCE_DIR}/${VULKAN_SDK_ARCH})

    # Vulkan Headers
    add_library(VulkanSDK::Headers INTERFACE IMPORTED)
    set_target_properties(VulkanSDK::Headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${VULKAN_SDK_ROOT}/include
    )

    # Slang
    add_library(VulkanSDK::Slang SHARED IMPORTED)
    set_target_properties(VulkanSDK::Slang PROPERTIES
        IMPORTED_IMPLIB ${VULKAN_SDK_ROOT}/lib/libslang.so
    )

    add_library(VulkanSDK::SlangCompiler SHARED IMPORTED)
    set_target_properties(VulkanSDK::SlangCompiler PROPERTIES
        IMPORTED_IMPLIB ${VULKAN_SDK_ROOT}/lib/libslang-compiler.so
    )

    link_directories(${VULKAN_SDK_ROOT}/lib)
endif()
