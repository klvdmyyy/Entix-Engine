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
if(NOT (WIN32 OR LINUX))
    message(FATAL_ERROR "Vulkan SDK installation doesn't supported on current platform.")
endif()

# Setting up Vulkan SDK version
set(VULKAN_SDK_VERSION "1.4.350.0" CACHE STRING "Vulakn SDK version")

if(WIN32)
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
    function(install_vulkan_sdk)
        set(WIN_DIR_ENV "$ENV{windir}")
        cmake_path(GET WIN_DIR_ENV PARENT_PATH VULKAN_SDK_PARENT_PATH)
        set(ENV{VULKAN_SDK} "${VULKAN_SDK_PARENT_PATH}VulkanSDK\\${VULKAN_SDK_VERSION}")

        message(STATUS "VulkanSDK path: $ENV{VULKAN_SDK}")

        if(NOT EXISTS $ENV{VULKAN_SDK})
            message(STATUS "Vulkan SDK ${VULKAN_SDK_VERSION} not found. Installing it!")
            set(INSTALLER_PATH "${CMAKE_BINARY_DIR}/VulkanSDK-Installer.exe")

            if(NOT EXISTS ${INSTALLER_PATH})
                message(STATUS "Download Vulkan SDK ${VULKAN_SDK_VERSION} Windows Installer")
                file(DOWNLOAD ${VULKAN_SDK_URL} ${INSTALLER_PATH} SHOW_PROGRESS)
            else()
                message(STATUS "Vulkan SDK Installer found!")
            endif()

            message(STATUS "Installing Vulkan SDK. This may take a few minutes...")
            execute_process(
                COMMAND ${INSTALLER_PATH} --accept-licenses --default-answer --confirm-command install
                RESULT_VARIABLE install_result
                OUTPUT_QUIET
            )

            if(NOT install_result EQUAL 0)
                message(FATAL_ERROR "Vulkan SDK installation failed!")
            endif()
        endif()
    endfunction()

    # Installation of Vulkan SDK on Windows is a little bit different.
    #
    # We need to run EXE installer. But before we do this, we want to check
    # if VulkanSDK already installed and it has right version.
    if(DEFINED ENV{VULKAN_SDK})
        # Get VulkanSDK version
        set(INSTALLED_VULKAN_SDK_PATH $ENV{VULKAN_SDK})
        cmake_path(GET INSTALLED_VULKAN_SDK_PATH FILENAME INSTALLED_VULKAN_SDK_VERSION_DIRECTORY)

        # Check VulkanSDK version
        if(NOT "${VULKAN_SDK_VERSION}" STREQUAL "${INSTALLED_VULKAN_SDK_VERSION_DIRECTORY}")
            message(WARNING "VulkanSDK version mismatch!")
            install_vulkan_sdk()
        endif()
    else()
        install_vulkan_sdk()
    endif()

    # Vulkan Headers
    add_library(VulkanSDK::Headers INTERFACE IMPORTED)
    set_target_properties(VulkanSDK::Headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES $ENV{VULKAN_SDK}/Include
    )

    # Slang
    add_library(VulkanSDK::Slang SHARED IMPORTED)
    set_target_properties(VulkanSDK::Slang PROPERTIES
        IMPORTED_IMPLIB $ENV{VULKAN_SDK}/Lib/slang.lib
    )

    add_library(VulkanSDK::SlangCompiler SHARED IMPORTED)
    set_target_properties(VulkanSDK::SlangCompiler PROPERTIES
        IMPORTED_IMPLIB $ENV{VULKAN_SDK}/Lib/slang-compiler.lib
    )

    link_directories($ENV{VULKAN_SDK}/Lib)
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
