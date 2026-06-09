# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Entix Engine")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries)

include(FetchContent)

FetchContent_Declare(
	CLI11
	GIT_REPOSITORY https://github.com/CLIUtils/CLI11
	GIT_TAG v2.6.2
)

FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL
	GIT_TAG release-3.4.10
)

FetchContent_MakeAvailable(SDL3)

# Include sub-projects.
add_subdirectory ("Runtime")
add_subdirectory ("Game")
