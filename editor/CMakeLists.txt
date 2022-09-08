add_executable(editor
	src/application.cpp
	src/mainframe.cpp
	src/viewport.cpp)

set_target_properties(editor PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_link_libraries(editor PRIVATE
	core
	wx::gl
	wx::core
	wx::base)
