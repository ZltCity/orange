#include <fmt/format.h>

#include <orange/gl/shader.hpp>

namespace orange::gl
{

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
