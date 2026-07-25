add_executable(Sandbox)

target_link_libraries(Sandbox PRIVATE Entix)

file(
    GLOB_RECURSE
    SANDBOX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

target_sources(Sandbox PRIVATE ${SANDBOX_SOURCES})