#pragma once

#include <functional>
#include <memory>

#include <glad/glad.h>

namespace orange::gl
{

class Handle
{
public:
	Handle() = default;
	Handle(GLuint handle, const std::function<void(GLuint)> &release);
	virtual ~Handle();

	operator bool() const;
	explicit operator GLuint() const;

private:
	std::shared_ptr<GLuint> ptr;
};

} // namespace orange::gl