#pragma once

#include <glad/glad.h> // OpenGL
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class ShaderPipeline {
public:
	ShaderPipeline(const char* vsSource, const char* fsSource);
	~ShaderPipeline();

	void bind();
	void unbind();
	void setUniformMatrix4f(const std::string& uniformName, const glm::mat4x4& mat);

private:
	int getUniformLocation(const std::string& name);

private:
	GLuint m_pipelineID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
};