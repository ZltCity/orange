#include <orange/gl/buffer.hpp>
#include <orange/gl/utils.hpp>

namespace orange::gl
{

GLuint createBuffer()
{
	auto handle = GLuint {};

	invoke(glCreateBuffers, 1, &handle);

	return handle;
}

Buffer::Buffer(GLsizeiptr size, const void *data, GLenum usage)
	: Handle(createBuffer(), [](GLuint handle) { glDeleteBuffers(1, &handle); })
{
	invoke(glNamedBufferData, Handle::operator GLuint(), size, data, usage);
}

} // namespace orange::gl
