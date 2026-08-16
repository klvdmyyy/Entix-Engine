#----------------------------------------+
# Copyright (C) 2026 Dmitriy Klementiev. |
#                                        |
# SPDX-License-Identifier: BSD-3-Clause  |
#----------------------------------------+

macro(enable_code_analysis_for TARGET_NAME)
    if(MSVC)
        set_target_properties(${TARGET_NAME} PROPERTIES
            VS_GLOBAL_RunCodeAnalysis true

            # Use visual studio core guidelines
            VS_GLOBAL_EnableMicrosoftCodeAnalysis true

            # Use clangtidy
            VS_GLOBAL_EnableClangTidyCodeAnalysis true
            VS_GLOBAL_ClangTidyChecks -checks=*,modernize-*,-modernize-use-trailing-return-type
        )
    else()
        message(WARNING "Current platform doesn't support code analysis in CMake!")
    endif()
endmacro()

macro(enable_all_warnings_for TARGET_PLATFORM)
    if(MSVC)
        target_compile_options(${TARGET_PLATFORM} PRIVATE
            /W4)
    else()
        target_compile_options(${TARGET_PLATFORM} PRIVATE
            -Wall -Wextra -Wpedantic)
    endif()
endmacro()

macro(force_utf8_for TARGET_PLATFORM)
    if(MSVC)
        target_compile_options(${TARGET_PLATFORM} PRIVATE
            /utf-8)
    else()
        target_compile_options(${TARGET_PLATFORM} PRIVATE
            -fexec-charset=UTF-8
            -finput-charset=UTF-8)
    endif()
endmacro()

macro(enable_sanitizers_for TARGET_PLATFORM)
    if(MSVC)
        target_compile_options(${TARGET_PLATFORM} PUBLIC
            $<$<CONFIG:Debug>:/fsanitize=address>)
    else()
        target_compile_options(${TARGET_PLATFORM} PUBLIC
            $<$<CONFIG:Debug>:-fsanitize=address,leak,undefined>)

        target_link_libraries(${TARGET_PLATFORM} PUBLIC
            $<$<CONFIG:Debug>:-fsanitize=address,leak,undefined>)
    endif()
endmacro()

macro(warnings_are_errors_for TARGET_PLATFORM)
    if(MSVC)
        target_compile_options(${TARGET_PLATFORM} PRIVATE
            /WX)
    else()
        target_compile_options(${TARGET_PLATFORM} PRIVATE
            -Werror)
    endif()
endmacro()
