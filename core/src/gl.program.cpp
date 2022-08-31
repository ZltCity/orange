#include <fmt/format.h>

#include <orange/gl/program.hpp>
#include <orange/gl/utils.hpp>

namespace orange::gl
{

Program::Program(const std::vector<Shader> &shaders)
	: Handle(invoke(glCreateProgram), [](GLuint handle) { glDeleteProgram(handle); })
{
	const auto handle = Handle::operator GLuint();

	for (const auto &shader : shaders)
	{
		invoke(glAttachShader, handle, shader.operator GLuint());
	}

	invoke(glLinkProgram, handle);

	GLint linkStatus {};

	invoke(glGetProgramiv, handle, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint logLength {};

		invoke(glGetProgramiv, handle, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(logLength, 0);

		invoke(glGetProgramInfoLog, handle, logLength, nullptr, log.data());

		throw std::runtime_error(fmt::format("Shader program link failed.\n{}", log));
	}
}

void Program::use() const
{
	invoke(glUseProgram, Handle::operator GLuint());
}

void Program::setUniform(const std::string &name, float value) const
{
	invoke(glUniform1f, invoke(glGetUniformLocation, Handle::operator GLuint(), name.c_str()), value);
}

void Program::setUniform(const std::string &name, const glm::vec2 &value) const
{
	invoke(
		glUniform2fv, invoke(glGetUniformLocation, Handle::operator GLuint(), name.c_str()), 1,
		reinterpret_cast<const GLfloat *>(&value));
}

} // namespace orange::gl
