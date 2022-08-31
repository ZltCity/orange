#pragma once

#include <filesystem>
#include <memory>

#include <orange/byte_buffer.hpp>

namespace orange
{

class AssetManager
{
public:
	explicit AssetManager(std::filesystem::path assetsPath);

	[[nodiscard]] std::shared_ptr<ByteBuffer> getAsset(const std::filesystem::path &path) const;

private:
	std::filesystem::path assetsPath;
};

} // namespace orange
