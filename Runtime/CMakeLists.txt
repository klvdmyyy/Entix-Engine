# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_library (EntixRuntime SHARED)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EntixRuntime PROPERTY CXX_STANDARD 23)
endif()

target_compile_definitions(EntixRuntime PRIVATE ERUNTIME_BUILD_DLL)

target_link_libraries(EntixRuntime PRIVATE SDL3-shared CLI11)

target_link_libraries(EntixRuntime PUBLIC spdlog::spdlog)

target_include_directories (EntixRuntime PUBLIC "Source/")

file(
	GLOB_RECURSE
	ENTIX_RUNTIME_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

target_sources(
	EntixRuntime
	PRIVATE
	${ENTIX_RUNTIME_SOURCES}
)

add_library(Entix::Runtime ALIAS EntixRuntime)

# TODO: Add tests and install targets if needed.
