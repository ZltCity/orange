#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include <orange/asset_manager.hpp>
#include <orange/gl/buffer.hpp>
#include <orange/gl/program.hpp>
#include <orange/gl/texture.hpp>
#include <orange/gl/vertex_array.hpp>
#include <orange/tile_map.hpp>

namespace orange
{

class Renderer
{
public:
	explicit Renderer(std::shared_ptr<AssetManager> assetManager);

	void setTileMap(size_t slot, const TileMap &tileMap);
	void renderTileMap(size_t slot, const glm::vec2 &screenSize, const glm::vec2 &cameraPosition, float scaleFactor);

	constexpr static size_t tileMapCount = 4;

private:
	void loadQuad();
	void loadShaders();
	void loadDefaultAtlas();

	std::shared_ptr<AssetManager> assetManager;

	gl::Buffer vbo;
	gl::VertexArray vao;
	gl::Program tileMapProgram;
	gl::Texture defaultAtlas;

	std::array<gl::Texture, tileMapCount> tileMaps;
};

} // namespace orange
