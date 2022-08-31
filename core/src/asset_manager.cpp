#include <fstream>
#include <stdexcept>

#include <fmt/format.h>

#include <orange/asset_manager.hpp>

namespace orange
{

AssetManager::AssetManager(std::filesystem::path assetsPath) : assetsPath(std::move(assetsPath))
{
}

std::shared_ptr<ByteBuffer> AssetManager::getAsset(const std::filesystem::path &path) const
{
	const auto fullPath = assetsPath / path;
	const auto fileSize = std::filesystem::file_size(fullPath);
	auto buffer = std::make_shared<ByteBuffer>(fileSize);
	auto stream = std::ifstream(fullPath, std::ios::binary);

	if (!stream.is_open())
	{
		throw std::runtime_error(fmt::format("Could not load file '{}'.", fullPath.string()));
	}

	stream.read(reinterpret_cast<char *>(buffer->data()), static_cast<std::streamsize>(fileSize));

	return buffer;
}

} // namespace orange
