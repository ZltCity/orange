#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <fmt/format.h>
#include <glm/glm.hpp>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <orange/gl/buffer.hpp>
#include <orange/gl/program.hpp>
#include <orange/gl/shader.hpp>
#include <orange/gl/texture.hpp>
#include <orange/gl/utils.hpp>
#include <orange/gl/vertex_array.hpp>

#include <orange/renderer.hpp>

#include "game.hpp"

namespace orange
{

struct Vertex
{
	glm::vec4 position, color;
	glm::vec2 texCoord;
};

std::string loadShaderSource(const std::filesystem::path &path)
{
	auto stream = std::ifstream(path, std::ios::binary);

	if (!stream.is_open())
	{
		throw std::runtime_error(fmt::format("Could not open file '{}'.", path.string()));
	}

	std::string source(std::filesystem::file_size(path), 0);

	stream.read(source.data(), static_cast<std::streamsize>(source.size()));

	return source;
}

std::vector<uint32_t> generateTileMap(uint32_t width, uint32_t height)
{
	auto data = std::vector<uint32_t>(width * height, 0u);

	for (uint32_t y = 0; y < height; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
			const auto tileX = uint8_t((x + y) % 2), tileY = uint8_t(0);

			data[x + y * width] = ((tileY << 8) + tileX);
		}
	}

	return data;
}

int Game::run(int argc, const char **argv)
{
	sf::Window window(
		sf::VideoMode({800u, 600u}), "My window", sf::Style::Default,
		sf::ContextSettings {24, 8, 0, 4, 6, sf::ContextSettings::Attribute::Core});

	if (!window.isOpen())
	{
		throw std::runtime_error("Could not open window.");
	}

	gladLoadGL();

	auto assetManager = std::make_shared<AssetManager>("./assets");
	auto renderer = Renderer(assetManager);

	//	const auto quad = std::vector<Vertex> {
	//		Vertex {glm::vec4 {-1.0f, -1.0f, 0.0f, 1.0f}, glm::vec4 {1.0f, 0.0f, 0.0f, 1.0f}, glm::vec2 {0.0f, 0.0f}},
	//		Vertex {glm::vec4 {1.0f, 1.0f, 0.0f, 1.0f}, glm::vec4 {0.0f, 1.0f, 0.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}},
	//		Vertex {glm::vec4 {-1.0f, 1.0f, 0.0f, 1.0f}, glm::vec4 {0.0f, 0.0f, 1.0f, 1.0f}, glm::vec2 {0.0f, 1.0f}},
	//		Vertex {glm::vec4 {-1.0f, -1.0f, 0.0f, 1.0f}, glm::vec4 {1.0f, 0.0f, 0.0f, 1.0f}, glm::vec2 {0.0f, 0.0f}},
	//		Vertex {glm::vec4 {1.0f, 1.0f, 0.0f, 1.0f}, glm::vec4 {0.0f, 1.0f, 0.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}},
	//		Vertex {glm::vec4 {1.0f, -1.0f, 0.0f, 1.0f}, glm::vec4 {0.0f, 0.0f, 1.0f, 1.0f}, glm::vec2 {1.0f, 0.0f}},
	//	};
	//	const auto vbo = gl::Buffer {static_cast<GLsizeiptr>(quad.size() * sizeof(Vertex)), quad.data(),
	// GL_STATIC_DRAW}; 	const auto vao = gl::VertexArray { 		vbo,
	//		{
	//			gl::VertexAttrib {0, 4, GL_FLOAT, GL_FALSE, 0},
	//			gl::VertexAttrib {1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4)},
	//			gl::VertexAttrib {2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 2},
	//		},
	//		sizeof(Vertex)};
	//	auto vertexShader = gl::Shader(GL_VERTEX_SHADER, loadShaderSource("assets/shaders/simple.vertex"));
	//	auto fragmentShader = gl::Shader(GL_FRAGMENT_SHADER, loadShaderSource("assets/shaders/simple.fragment"));
	//	auto shaderProgram = gl::Program({vertexShader, fragmentShader});
	//
	//	int imageWidth {}, imageHeight {}, bpp {};
	//	const auto imageData = stbi_load("assets/images/x-files.png", &imageWidth, &imageHeight, &bpp, 4);
	//
	//	if (imageData == nullptr)
	//	{
	//		throw std::runtime_error("Could not load image.");
	//	}
	//
	//	auto atlas = gl::Texture(1, GL_RGBA8, imageWidth, imageHeight);
	//
	//	atlas.subImage(0, 0, 0, imageWidth, imageHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	//
	//	//	const auto tileMapData = generateTileMap(256, 256);
	int tileMapWidth {}, tileMapHeight {}, tileMapBpp {};
	const auto tileMapData = stbi_load("assets/images/test-tilemap.png", &tileMapWidth, &tileMapHeight, &tileMapBpp, 4);

	if (tileMapData == nullptr)
	{
		throw std::runtime_error("Could not load tile map.");
	}

	auto tileMap = TileMap(tileMapWidth, tileMapHeight);

	for (uint32_t y = 0; y < tileMapHeight; ++y)
	{
		for (uint32_t x = 0; x < tileMapWidth; ++x)
		{
			tileMap.setTile(x, y, Tile {reinterpret_cast<const uint32_t *>(tileMapData)[x + y * tileMapWidth]});
		}
	}

	renderer.setTileMap(0, tileMap);
	//
	//	auto tileMap = gl::Texture(1, GL_RGBA8UI, tileMapWidth, tileMapHeight);
	//
	//	tileMap.subImage(0, 0, 0, tileMapWidth, tileMapHeight, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, tileMapData);

	auto screenSize = glm::vec2 {800, 600};
	//	auto rad = float {};
	auto offset = glm::vec2 {};
	auto running = true;

	// run the program as long as the window is open
	while (running)
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event {};

		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				screenSize = glm::vec2 {event.size.width, event.size.height};
				// adjust the viewport when the window is resized
				gl::invoke(glViewport, 0, 0, event.size.width, event.size.height);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			offset += glm::vec2 {-1.0f, 0.0f};
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			offset += glm::vec2 {1.0f, 0.0f};
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			offset += glm::vec2 {0.0f, 1.0f};
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			offset += glm::vec2 {0.0f, -1.0f};
		}

		gl::invoke(glClearColor, 0.0f, 0.0f, 0.0f, 0.0f);
		gl::invoke(glClear, GL_COLOR_BUFFER_BIT);

		//		vao.bind();
		//		atlas.bind(0);
		//		tileMap.bind(1);
		//		shaderProgram.use();
		//		shaderProgram.setUniform("screenSize", screenSize);
		//		shaderProgram.setUniform("tileMapSize", glm::vec2 {tileMapWidth, tileMapHeight});
		//		//		shaderProgram.setUniform("offset", glm::vec2 {glm::sin(rad), glm::cos(rad)} * 500.0f);
		//		shaderProgram.setUniform("offset", offset * 10.0f);
		//
		//		//		rad += 0.01f;
		std::cout << offset.x << ", " << offset.y << std::endl;
		renderer.renderTileMap(0, screenSize, offset * 10.0f, 10.0f);

		//		gl::invoke(glDrawArrays, GL_TRIANGLES, 0, quad.size());

		window.display();
	}

	return 0;
}

} // namespace orange
