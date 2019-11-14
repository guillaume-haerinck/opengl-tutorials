#include "ShaderPipeline.hpp"

#include "common/gl-exception.h"
#include <spdlog/spdlog.h>

ShaderPipeline::ShaderPipeline(const char* vsSource, const char* fsSource) {
	// ------------------ Vertex shader
	unsigned int vs;
	int success;
	char infoLog[512];
	{
		vs = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(vs, 1, &vsSource, NULL));
		GLCall(glCompileShader(vs));

		// Check compilation
		GLCall(glGetShaderiv(vs, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(vs, 512, NULL, infoLog));
			spdlog::critical("[VertexShader] Compilation failed : {}", infoLog);
			debug_break();
		}
	}

	// ------------------ Fragment shader
	unsigned int fs;
	{
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(fs, 1, &fsSource, NULL));
		GLCall(glCompileShader(fs));

		// Check compilation
		int success;
		GLCall(glGetShaderiv(fs, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(fs, 512, NULL, infoLog));
			spdlog::critical("[FragmentShader] Compilation failed : {}", infoLog);
			debug_break();
		}
	}

	// ------------------ Pipeline
	{
		m_pipelineID = glCreateProgram();
		GLCall(glAttachShader(m_pipelineID, vs));
		GLCall(glAttachShader(m_pipelineID, fs));
		GLCall(glLinkProgram(m_pipelineID));

		// Check compilation
		GLCall(glGetProgramiv(m_pipelineID, GL_LINK_STATUS, &success));
		if (!success) {
			GLCall(glGetProgramInfoLog(m_pipelineID, 512, NULL, infoLog));
			spdlog::critical("[Pipeline] Link failed : {}", infoLog);
			debug_break();
		}

		// Delete useless data
		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
	}
}

ShaderPipeline::~ShaderPipeline() {
	GLCall(glDeleteProgram(m_pipelineID));
}

void ShaderPipeline::bind() {
	GLCall(glUseProgram(m_pipelineID));
}

void ShaderPipeline::setUniformMat4f(const std::string& uniformName, const glm::mat4x4& mat) {
	GLCall(glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, &mat[0][0]));
}

int ShaderPipeline::getUniformLocation(const std::string& name) {
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_pipelineID, name.c_str()));
	if (location == -1) {
		spdlog::warn("[Shader] uniform '{}' doesn't exist !", name);
		debug_break();
	}
	m_uniformLocationCache[name] = location;
	return location;
}