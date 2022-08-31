add_executable(game
	src/game.cpp
	src/main.cpp)

set_target_properties(game PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(game PRIVATE
	core
	sfml-graphics)
