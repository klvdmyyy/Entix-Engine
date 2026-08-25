# Helper function to declare game/program built on top of Entix Runtime.
#
# For now it provides only basic declaration of target but
# I think we can make more complex functional to simplify
# Entix Engine usage and make it "same everywhere"
#
# I don't really love complexity of CMake. Why I must write
# hundred lines of code to declare executable ?!
#
# This function can be also used in code-generation laterly. We don't
# need to touch code generator everytime I would like to make some changes.
function(add_entix_executable TARGET_NAME)
    # Include basic Entix Utilities for CMake
    include(utilities)

    # Define Win32 executable if it possible
    if(WIN32)
        # add_executable(${TARGET_NAME} WIN32)
        add_executable(${TARGET_NAME})
    else()
        add_executable(${TARGET_NAME})
    endif()

    # Enable code analysis for target (clang-tidy, visual-studio and so on)
    enable_code_analysis_for(${TARGET_NAME})

    # Link with engine runtime
    target_link_libraries(${TARGET_NAME}
        PRIVATE
            Entix)

    # Source files must be at the same place in every project
    file(GLOB_RECURSE
        ENTIX_EXECUTABLE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    )

    # Attach source files to target
    target_sources(${TARGET_NAME} PRIVATE ${ENTIX_EXECUTABLE_SOURCES})
endfunction()

add_entix_executable(Sandbox)
