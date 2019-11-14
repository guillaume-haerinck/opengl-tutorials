#include "ShaderPipeline.hpp"

#include "common/gl-exception.h"
#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>

ShaderPipeline::ShaderPipeline(const std::string& vertexFilepath, const std::string& fragmentFilepath) {
	// ------------------ Vertex shader
	unsigned int vs;
	int success;
	char infoLog[512];
	{
		std::string vsSource = readFile(vertexFilepath);
		const char* vsSourceCstr = vsSource.c_str();
		vs = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(vs, 1, &vsSourceCstr, NULL));
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
		std::string fsSource = readFile(fragmentFilepath);
		const char* fsSourceCstr = fsSource.c_str();
		fs = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(fs, 1, &fsSourceCstr, NULL));
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

std::string ShaderPipeline::readFile(const std::string& filepath) {
	// Open file
	std::ifstream stream(filepath);
	if (!stream.is_open()) {
		spdlog::warn("Failed to open file : |{}|", filepath);
		return "";
	}

	// Read line by line and put it in a string
	std::string str = "";
	std::string tempLine = "";
	while (getline(stream, tempLine)) {
		str += tempLine + '\n';
	}
	stream.close();
	return str;
}