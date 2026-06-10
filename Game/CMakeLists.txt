if(WIN32S)
	add_executable(Game WIN32)
else()
	add_executable(Game)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Game PROPERTY CXX_STANDARD 23)
endif()

target_link_libraries(Game PRIVATE Entix::Runtime)

file(
	GLOB_RECURSE
	GAME_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
)

target_sources(
	Game
	PRIVATE
	${GAME_SOURCES}
)