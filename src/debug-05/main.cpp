#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

#include "common/app.h"
#include "common/gl-exception.h"
#include "common/square-data.h"

std::unordered_map<std::string, int> uniformLocationCache;

int getUniformLocation(const std::string& name, unsigned int pipeline) {
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
		return uniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(pipeline, name.c_str()));
	if (location == -1) {
		spdlog::warn("[Shader] uniform '{}' doesn't exist !", name);
        debug_break();
	}
	uniformLocationCache[name] = location;
	return location;
}

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

    // ------------------ Vertex Buffer 1
    unsigned int posVB;
    {
        GLCall(glGenBuffers(1, &posVB));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVB));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(squareData::positions), squareData::positions, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
    
    // ------------------ Vertex Array
    unsigned int vao;
    {
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        // Vertex input description
        {
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVB));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));
        }
        
        GLCall(glBindVertexArray(0));
    }

    // ------------------ Index buffer
    unsigned int ib;
    {
        GLCall(glGenBuffers(1, &ib));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareData::indices), squareData::indices, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    // ------------------ Vertex shader
    unsigned int vs;
    int success;
    char infoLog[512];
    {
        const char* vsSource = R"(#version 330 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 uModel;
            uniform mat4 uViewProj;

            void main() {
                gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
            }
        )";
        
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
        const char* fsSource = R"(#version 330 core
            out vec4 FragColor;

            void main() {
                FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
            } 
        )";
        
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
    unsigned int pipeline;
    {
        pipeline = glCreateProgram();
        GLCall(glAttachShader(pipeline, vs));
        GLCall(glAttachShader(pipeline, fs));
        GLCall(glLinkProgram(pipeline));

        // Check compilation
        GLCall(glGetProgramiv(pipeline, GL_LINK_STATUS, &success));
        if (!success) {
            GLCall(glGetProgramInfoLog(pipeline, 512, NULL, infoLog));
            spdlog::critical("[Pipeline] Link failed : {}", infoLog);
            debug_break();
        }

        // Delete useless data
        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));
    }

    // ------------------ Uniforms
    int modelMatUniform;
    glm::mat4 modelMat = glm::mat4(1.0f);
    int viewProjMatUniform;
    glm::mat4 viewProjMat = glm::mat4(1.0f);
    {
        GLCall(glUseProgram(pipeline));
        GLCall(glUniformMatrix4fv(getUniformLocation("uModel", pipeline), 1, GL_FALSE, &modelMat[0][0]));
        GLCall(glUniformMatrix4fv(getUniformLocation("uViewProj", pipeline), 1, GL_FALSE, &viewProjMat[0][0]));
        GLCall(glUseProgram(0));
    }

    float counter = 0.0f;
    while (app.isRunning()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: app.exit();

            default: break;
            };
        }

        counter += 0.05f;
        if (counter > 100) {
            counter = 0;
        }

        app.beginFrame();

        // Update uniforms
        GLCall(glUseProgram(pipeline));
        {
            modelMat = glm::rotate(glm::mat4(1.0f), counter, glm::vec3(0, 1, 0));
            GLCall(glUniformMatrix4fv(getUniformLocation("uModel", pipeline), 1, GL_FALSE, &modelMat[0][0]));
        }
        {
            glm::mat4 viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
            glm::mat4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
            viewProjMat = projMat * viewMat;
            GLCall(glUniformMatrix4fv(getUniformLocation("uViewProj", pipeline), 1, GL_FALSE, &viewProjMat[0][0]));
        }

        // Draw call
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
        GLCall(glDrawElements(GL_TRIANGLES, std::size(squareData::indices), GL_UNSIGNED_SHORT, (void*) 0));

        app.endFrame();
    }
    
    return 0;
}
