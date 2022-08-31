#pragma once

#include <stdexcept>
#include <string>

#include <fmt/format.h>
#include <glad/glad.h>

namespace orange::gl
{

std::string toString(GLenum error);

template<typename F, typename... Args>
auto invoke(F f, Args... args) -> typename std::invoke_result<F, Args...>::type
{
	using Result = typename std::invoke_result<F, Args...>::type;

	auto glassert = []() {
		GLenum error = glGetError();

		if (error != GL_NO_ERROR)
			throw std::runtime_error(fmt::format("OpenGL error: {}.", toString(error)));
	};

	if constexpr (std::is_void_v<Result>)
	{
		f(args...);
		glassert();
	}
	else
	{
		Result result = f(args...);

		glassert();

		return result;
	}
}

} // namespace orange::gl
