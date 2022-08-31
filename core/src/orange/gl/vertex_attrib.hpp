#pragma once

#include <glad/glad.h>

namespace orange::gl
{

struct VertexAttrib
{
	GLuint attribindex;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLuint relativeoffset;
};

} // namespace orange::gl
