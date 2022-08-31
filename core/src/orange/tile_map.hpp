#pragma once

#include <cstdint>
#include <tuple>
#include <vector>

#include <orange/tile.hpp>

namespace orange
{

class TileMap
{
public:
	TileMap(uint32_t width, uint32_t height);

	void setTile(uint32_t x, uint32_t y, const Tile &tile);

	[[nodiscard]] std::tuple<uint32_t, uint32_t> getSize() const;
	[[nodiscard]] const Tile *getData() const;

private:
	uint32_t width, height;
	std::vector<Tile> tiles;
};

} // namespace orange
