#pragma once

#include <orange/byte_buffer.hpp>
#include <orange/gl/handle.hpp>

namespace orange::gl
{

class Shader : public Handle
{
public:
	Shader() = default;
	Shader(GLenum type, const std::string &source);
	Shader(GLenum type, const ByteBuffer &buffer);

private:
	void compile(const GLchar *source, GLint length);
};

} // namespace orange::gl
