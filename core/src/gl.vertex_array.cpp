#include <orange/gl/utils.hpp>
#include <orange/gl/vertex_array.hpp>

namespace orange::gl
{

GLuint createVertexArray()
{
	auto handle = GLuint {};

	invoke(glCreateVertexArrays, 1, &handle);

	return handle;
}

VertexArray::VertexArray(const Buffer &buffer, const std::vector<VertexAttrib> &attributes, GLsizei stride)
	: Handle(createVertexArray(), [](GLuint handle) { glDeleteVertexArrays(1, &handle); })
{
	const auto handle = Handle::operator GLuint();

	for (const auto &attr : attributes)
	{
		invoke(glVertexArrayAttribBinding, handle, attr.attribindex, 0);
		invoke(
			glVertexArrayAttribFormat, handle, attr.attribindex, attr.size, attr.type, attr.normalized,
			attr.relativeoffset);
		invoke(glEnableVertexArrayAttrib, handle, attr.attribindex);
	}

	invoke(glVertexArrayVertexBuffer, handle, 0, buffer.operator GLuint(), 0, stride);
}

void VertexArray::bind() const
{
	invoke(glBindVertexArray, Handle::operator GLuint());
}

} // namespace orange::gl
