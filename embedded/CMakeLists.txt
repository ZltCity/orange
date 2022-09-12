add_custom_target(header_generator)
add_custom_command(TARGET header_generator
	BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/src/resources.cpp
	PRE_BUILD COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/embed.py
	--embedded-dir ${CMAKE_SOURCE_DIR}/embedded/resources
	--output-dir ${CMAKE_CURRENT_BINARY_DIR}
	--extensions "vertex, fragment, png, svg")

add_library(embedded STATIC
	${CMAKE_CURRENT_BINARY_DIR}/src/resources.cpp)

target_include_directories(embedded PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}/include)

add_dependencies(embedded header_generator)
