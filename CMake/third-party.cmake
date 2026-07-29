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