#include <orange/gl/handle.hpp>

namespace orange::gl
{

Handle::Handle(GLuint handle, const std::function<void(GLuint)> &release)
	: ptr(new GLuint(handle), [release](auto p) {
		  release(*p);
		  delete p;
	  })
{
}

Handle::~Handle()
{
}

Handle::operator bool() const
{
	return ptr != nullptr;
}

Handle::operator GLuint() const
{
	if (ptr == nullptr)
		throw std::runtime_error("Invalid handle.");
	else
		return *ptr;
}

} // namespace orange::gl