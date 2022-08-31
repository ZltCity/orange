#include <orange/gl/texture.hpp>
#include <orange/gl/utils.hpp>

namespace orange::gl
{

GLuint createTexture(GLenum target)
{
	auto handle = GLuint {};

	invoke(glCreateTextures, target, 1, &handle);

	return handle;
}

Texture::Texture(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
	: Handle(createTexture(GL_TEXTURE_2D), [](GLuint handle) { glDeleteTextures(1, &handle); })
{
	const auto handle = Handle::operator GLuint();

	invoke(glTextureStorage2D, handle, levels, internalformat, width, height);
	invoke(glTextureParameteri, handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	invoke(glTextureParameteri, handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::subImage(
	GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
	const void *pixels)
{
	invoke(
		glTextureSubImage2D, Handle::operator GLuint(), level, xoffset, yoffset, width, height, format, type, pixels);
}

void Texture::setParameter(GLenum pname, GLint param)
{
	invoke(glTextureParameteri, Handle::operator GLuint(), pname, param);
}

void Texture::bind(GLuint unit) const
{
	invoke(glBindTextureUnit, unit, Handle::operator GLuint());
}

glm::ivec2 Texture::getSize(GLint level) const
{
	const auto handle = Handle::operator GLuint();
	auto width = GLint {}, height = GLint {};

	gl::invoke(glGetTextureLevelParameteriv, handle, level, GL_TEXTURE_WIDTH, &width);
	gl::invoke(glGetTextureLevelParameteriv, handle, level, GL_TEXTURE_HEIGHT, &height);

	return {width, height};
}

} // namespace orange::gl
