#pragma once

#include <vector>

#include <orange/gl/buffer.hpp>
#include <orange/gl/vertex_attrib.hpp>

namespace orange::gl
{

class VertexArray : public Handle
{
public:
	VertexArray() = default;
	VertexArray(const Buffer &buffer, const std::vector<VertexAttrib> &attributes, GLsizei stride);

	void bind() const;
};

} // namespace orange::gl
