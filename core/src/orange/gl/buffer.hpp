#pragma once

#include <orange/gl/handle.hpp>

namespace orange::gl
{

class Buffer : public Handle
{
public:
	Buffer() = default;
	Buffer(GLsizeiptr size, const void *data, GLenum usage);
};

} // namespace orange::gl
