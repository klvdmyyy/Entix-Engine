#----------------------------------------+
# Copyright (C) 2026 Dmitriy Klementiev. |
#                                        |
# SPDX-License-Identifier: BSD-3-Clause  |
#----------------------------------------+

include(FetchContent)

if(NOT TARGET SDL3-shared)
    FetchContent_Declare(SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-3.4.14
    )

    FetchContent_MakeAvailable(SDL3)
    if(NOT TARGET ThirdParty::SDL3)
        add_library(ThirdParty::SDL3 ALIAS SDL3-shared)
    endif()
endif()

if(NOT TARGET EnTT)
    FetchContent_Declare(EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt
        GIT_TAG v3.16.0
    )

    FetchContent_MakeAvailable(EnTT)

    if(NOT TARGET ThirdParty::EnTT)
        add_library(ThirdParty::EnTT ALIAS EnTT)
    endif()
endif()

if(NOT TARGET glm)
    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.3
    )

    FetchContent_MakeAvailable(glm)

    if(NOT TARGET ThirdParty::GLM)
        add_library(ThirdParty::GLM ALIAS glm)
    endif()
endif()

if(NOT TARGET nlohmann_json)
    FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.12.0
    )

    FetchContent_MakeAvailable(nlohmann_json)

    if(NOT TARGET ThirdParty::JSON)
        add_library(ThirdParty::JSON ALIAS nlohmann_json)
    endif()
endif()

if(NOT TARGET CLI11)
    FetchContent_Declare(CLI11
        GIT_REPOSITORY https://github.com/klvdmyyy/CLI11
        GIT_TAG "origin/main"
    )

    FetchContent_MakeAvailable(CLI11)

    if(NOT TARGET ThirdParty::CLI11)
        add_library(ThirdParty::CLI11 ALIAS CLI11)
    endif()
endif()

if(NOT TARGET TracyClient)
    FetchContent_Declare(Tracy
        GIT_REPOSITORY https://github.com/klvdmyyy/tracy
    )

    set(TRACY_ON_DEMAND OFF CACHE BOOL "" FORCE)
    set(TRACY_STATIC OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(Tracy)

    if(NOT TARGET ThirdParty::TracyClient)
        add_library(ThirdParty::TracyClient ALIAS TracyClient)
    endif()
endif()
