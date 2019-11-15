#include "ShaderPipeline.hpp"

#include "common/gl-exception.h"
#include <spdlog/spdlog.h>

ShaderPipeline::ShaderPipeline(const char* vsSource, const char* fsSource) {

}

ShaderPipeline::~ShaderPipeline() {

}

void ShaderPipeline::bind() {

}

void ShaderPipeline::unbind() {

}

void ShaderPipeline::setUniformMatrix4f(const std::string& uniformName, const glm::mat4x4& mat) {

}

int ShaderPipeline::getUniformLocation(const std::string& name) {

}