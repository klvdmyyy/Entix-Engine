add_executable(Sandbox)

if(MSVC)
    set_target_properties(Sandbox PROPERTIES
        VS_GLOBAL_RunCodeAnalysis true

        # Use visual studio core guidelines
        VS_GLOBAL_EnableMicrosoftCodeAnalysis true

        # Use clangtidy
        VS_GLOBAL_EnableClangTidyCodeAnalysis true
        VS_GLOBAL_ClangTidyChecks -checks=*,modernize-*,-modernize-use-trailing-return-type
    )
endif()

target_link_libraries(Sandbox PRIVATE Entix)

file(
    GLOB_RECURSE
    SANDBOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

target_sources(Sandbox PRIVATE ${SANDBOX_SOURCES})