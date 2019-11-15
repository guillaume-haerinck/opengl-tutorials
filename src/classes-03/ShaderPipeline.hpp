#pragma once

#include <glad/glad.h> // OpenGL
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class ShaderPipeline {
public:
	ShaderPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~ShaderPipeline();

	void bind();
	void unbind();
	void setUniformMat4f(const std::string& uniformName, const glm::mat4x4& mat);

private:
	int getUniformLocation(const std::string& name);
	std::string readFile(const std::string& filepath);

private:
	GLuint m_pipelineID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
};