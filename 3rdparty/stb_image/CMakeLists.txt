FetchContent_Declare(stb_image
	GIT_REPOSITORY https://github.com/nothings/stb.git)
FetchContent_Populate(stb_image)

add_library(stb_image INTERFACE)
add_library(stb::stb ALIAS stb_image)

target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})
