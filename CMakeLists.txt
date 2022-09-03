cmake_minimum_required(VERSION 3.20)

project(Orange
	C
	CXX)

include(FetchContent)

if (MSVC)
	add_compile_options(/W4 /wd4201)
elseif (GNU OR Clang)
	add_compile_options(-Wall -Wextra -Wpedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types)
endif ()

add_subdirectory(3rdparty/format)
add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/json)
add_subdirectory(3rdparty/sfml)
add_subdirectory(3rdparty/stb_image)
add_subdirectory(3rdparty/wxWidgets)

add_subdirectory(core)
add_subdirectory(editor)
add_subdirectory(embedded)
add_subdirectory(game)
