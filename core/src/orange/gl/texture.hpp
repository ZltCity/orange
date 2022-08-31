#pragma once

#include <glm/glm.hpp>

#include <orange/gl/handle.hpp>

namespace orange::gl
{

class Texture : public Handle
{
public:
	Texture() = default;
	Texture(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

	void subImage(
		GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
		const void *pixels);
	void setParameter(GLenum pname, GLint param);
	void bind(GLuint unit) const;

	[[nodiscard]] glm::ivec2 getSize(GLint level) const;
};

} // namespace orange::gl
