#pragma once

#include <string_view>

#include <orange/byte_buffer.hpp>
#include <orange/gl/handle.hpp>
#include <orange/gl/utils.hpp>

namespace orange::gl
{

class Shader : public Handle
{
public:
	Shader() = default;
	template<class T>
	Shader(GLenum type, const T &source);

private:
	void compile(const GLchar *source, GLint length);
};

template<class T>
Shader::Shader(GLenum type, const T &source)
	: Handle(invoke(glCreateShader, type), [](GLuint handle) { glDeleteShader(handle); })
{
	compile(reinterpret_cast<const GLchar *>(source.data()), static_cast<GLint>(source.size()));
}

} // namespace orange::gl
