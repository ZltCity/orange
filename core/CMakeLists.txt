add_library(core STATIC
	src/asset_manager.cpp
	src/gl.buffer.cpp
	src/gl.handle.cpp
	src/gl.program.cpp
	src/gl.shader.cpp
	src/gl.texture.cpp
	src/gl.utils.cpp
	src/gl.vertex_array.cpp
	src/glad.c
	src/renderer.cpp
	src/tile_map.cpp)

set_target_properties(core PROPERTIES
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON)

target_include_directories(core PUBLIC
	src)
target_link_libraries(core PUBLIC
	embedded
	fmt::fmt-header-only
	glm::glm
	nlohmann_json::nlohmann_json
	stb::stb)

if (UNIX)
	target_link_libraries(core PRIVATE
		dl)
elseif (WIN32)
	target_link_libraries(core PRIVATE
		opengl32)
endif ()
