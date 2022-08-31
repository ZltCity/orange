#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <orange/gl/shader.hpp>

namespace orange::gl
{

class Program : public Handle
{
public:
	Program() = default;
	explicit Program(const std::vector<Shader> &shaders);

	void use() const;

	void setUniform(const std::string &name, float value) const;
	void setUniform(const std::string &name, const glm::vec2 &value) const;
};

} // namespace orange::gl
