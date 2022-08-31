#include <fmt/format.h>

#include <orange/gl/shader.hpp>
#include <orange/gl/utils.hpp>

namespace orange::gl
{

Shader::Shader(GLenum type, const std::string &source)
	: Handle(invoke(glCreateShader, type), [](GLuint handle) { glDeleteShader(handle); })
{
	compile(reinterpret_cast<const GLchar *>(source.data()), static_cast<GLint>(source.size()));
}

Shader::Shader(GLenum type, const ByteBuffer &buffer)
	: Handle(invoke(glCreateShader, type), [](GLuint handle) { glDeleteShader(handle); })
{
	compile(reinterpret_cast<const GLchar *>(buffer.data()), static_cast<GLint>(buffer.size()));
}

void Shader::compile(const GLchar *source, GLint length)
{
	const auto handle = Handle::operator GLuint();

	invoke(glShaderSource, handle, 1, &source, &length);
	invoke(glCompileShader, handle);

	GLint compileStatus {};

	invoke(glGetShaderiv, handle, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE)
	{
		GLint logLength {};

		invoke(glGetShaderiv, handle, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, 0);

		invoke(glGetShaderInfoLog, handle, logLength, nullptr, log.data());

		throw std::runtime_error(fmt::format("Shader compilation failed.\n{}.", log));
	}
}

} // namespace orange::gl
