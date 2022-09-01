#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <orange/gl/utils.hpp>
#include <orange/renderer.hpp>

#include <embedded/resources.hpp>

namespace orange
{

struct Vertex
{
	glm::vec4 position;
	glm::vec2 texCoord;
};

Renderer::Renderer(std::shared_ptr<AssetManager> assetManager) : assetManager(std::move(assetManager))
{
	loadQuad();
	loadShaders();
	loadDefaultAtlas();
}

void Renderer::setTileMap(size_t slot, const TileMap &tileMap)
{
	if (slot >= tileMaps.size())
	{
		throw std::out_of_range("Invalid slot.");
	}

	const auto [width, height] = tileMap.getSize();
	auto texture = gl::Texture(1, GL_RGBA8UI, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

	texture.subImage(
		0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_RGBA_INTEGER, GL_UNSIGNED_BYTE,
		tileMap.getData());

	tileMaps[slot] = texture;
}

void Renderer::renderTileMap(
	size_t slot, const glm::vec2 &screenSize, const glm::vec2 &cameraPosition, float scaleFactor)
{
	if (slot >= tileMaps.size())
	{
		throw std::out_of_range("Invalid slot.");
	}

	const auto &tileMap = tileMaps[slot];

	vao.bind();
	defaultAtlas.bind(0);
	tileMap.bind(1);
	tileMapProgram.use();
	tileMapProgram.setUniform("screenSize", screenSize);
	tileMapProgram.setUniform("tileMapSize", tileMap.getSize(0));
	tileMapProgram.setUniform("offset", cameraPosition);
	tileMapProgram.setUniform("scaleFactor", scaleFactor);

	gl::invoke(glDrawArrays, GL_TRIANGLES, 0, 6);
}

void Renderer::loadQuad()
{
	const auto quad = std::vector<Vertex> {
		Vertex {glm::vec4 {-1.0f, -1.0f, 0.0f, 1.0f}, glm::vec2 {0.0f, 0.0f}},
		Vertex {glm::vec4 {1.0f, 1.0f, 0.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}},
		Vertex {glm::vec4 {-1.0f, 1.0f, 0.0f, 1.0f}, glm::vec2 {0.0f, 1.0f}},
		Vertex {glm::vec4 {-1.0f, -1.0f, 0.0f, 1.0f}, glm::vec2 {0.0f, 0.0f}},
		Vertex {glm::vec4 {1.0f, 1.0f, 0.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}},
		Vertex {glm::vec4 {1.0f, -1.0f, 0.0f, 1.0f}, glm::vec2 {1.0f, 0.0f}},
	};
	vbo = gl::Buffer {static_cast<GLsizeiptr>(quad.size() * sizeof(Vertex)), quad.data(), GL_STATIC_DRAW};
	vao = gl::VertexArray {
		vbo,
		{
			gl::VertexAttrib {0, 4, GL_FLOAT, GL_FALSE, 0},
			gl::VertexAttrib {1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec4)},
		},
		sizeof(Vertex)};
}

void Renderer::loadShaders()
{
	tileMapProgram = gl::Program({
		gl::Shader(GL_VERTEX_SHADER, embedded::getResource(embedded::ResourceID::TILE_MAP_VERTEX)),
		gl::Shader(GL_FRAGMENT_SHADER, embedded::getResource(embedded::ResourceID::TILE_MAP_FRAGMENT)),
	});
}

void Renderer::loadDefaultAtlas()
{
	const auto asset = assetManager->getAsset("./images/default.atlas.png");
	auto width = int {}, height = int {}, channels = int {};
	const auto data = stbi_load_from_memory(
		reinterpret_cast<stbi_uc *>(asset->data()), static_cast<int>(asset->size()), &width, &height, &channels, 4);

	if (data == nullptr)
	{
		throw std::runtime_error("Could not load default atlas.");
	}

	defaultAtlas = gl::Texture(1, GL_RGBA8, width, height);

	defaultAtlas.subImage(0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

} // namespace orange
