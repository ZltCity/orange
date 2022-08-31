#include <stdexcept>

#include <orange/tile_map.hpp>

namespace orange
{

TileMap::TileMap(uint32_t width, uint32_t height) : width(width), height(height), tiles(width * height)
{
}

void TileMap::setTile(uint32_t x, uint32_t y, const Tile &tile)
{
	if (x >= width || y >= height)
	{
		throw std::out_of_range("Out of range.");
	}

	tiles[x + y * width] = tile;
}

std::tuple<uint32_t, uint32_t> TileMap::getSize() const
{
	return {width, height};
}

const Tile *TileMap::getData() const
{
	return tiles.data();
}

} // namespace orange
