# Abstracting OpenGL objects

Moving on from the Debug exercises, which lead us to drawing a cube, we will now try to draw several of them with different shader-pipelines. In order to do so without too much pain, we first need to put all that code into classes to be able to reuse it easily.

## Table of Contents

+ [Shaders pipeline](#01---shaders-pipeline)
+ [Catch Shader errors](#02---50-shades-of-errors)
+ [Debug buffer data](#03---buffers-everywhere)
+ [Uniforms handling](#04---uniforms-in-shape)
+ [Everything together](#05---cube-mesh-and-the-universe)

___

## 01 - Shaders Pipeline

> Open shaders from files, compile them into a pipepline, and bind uniforms in one line !

### 🧐👌 Moving the code into a class !

First, go to the empty ShaderPipeline.hpp file and declare your class : we need a constructor, a destructor, and what else ?
Try to fill in as much as you can ! :)
What are the methods you can think of ? Which arguments does our constructor require ? What are the private member(s) we need to store to access our pipeline on the GPU ?

Then start looking in the main and cut-paste the code in the body of the corresponding methods, replacing code in the main by the creation and calls to an instance of ShaderPipeline.
You will probably have to add methods you didn't think of, that's normal ;)

<details><summary>Solution</summary>

<details><summary>ShaderPipeline.hpp</summary>

```C++

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
	void setUniformMat4f(const std::string& uniformName, const glm::mat4x4& mat);

private:
	int getUniformLocation(const std::string& name);

private:
	GLuint m_pipelineID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
};

```

PS : make sure you pass your strings as const references ! ("const std::string&" instead of "std::string")

</details>

<details><summary>ShaderPipeline.cpp</summary>

```C++

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

```

</details>

<details><summary>main.cpp</summary>

```C++
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "common/app.h"
#include "common/gl-exception.h"
#include "common/square-data.h"

#include "ShaderPipeline.hpp"

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

	// ------------------ Shader pipeline

	const char* vsSource = R"(#version 330 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 uModel;
            uniform mat4 uViewProj;

            void main() {
                gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
            }
        )";


	const char* fsSource = R"(#version 330 core
            out vec4 FragColor;

            void main() {
                FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
            } 
        )";

	ShaderPipeline shaderPipeline(vsSource, fsSource);

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
		shaderPipeline.bind();
        {
            glm::mat4x4 modelMat = glm::rotate(glm::mat4(1.0f), counter, glm::vec3(0, 1, 0));
			shaderPipeline.setUniformMat4f("uModel", modelMat);
        }
        {
            glm::mat4x4 viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
            glm::mat4x4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
            glm::mat4x4 viewProjMat = projMat * viewMat;
			shaderPipeline.setUniformMat4f("uViewProj", viewProjMat);
        }

        // Draw call
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
        GLCall(glDrawElements(GL_TRIANGLES, std::size(squareData::indices), GL_UNSIGNED_SHORT, (void*) 0));

        app.endFrame();
    }
    
    return 0;
}
```

</details>

</details>

### 🧐 Improving the code : read shaders from a file instead of writting it as a string in the .cpp

#### 🤔 The problem

It's **** illisible, you have no syntax highlight, nothing :/ + the code cannot be reused for several shaders.

#### 👌 The solution

We will use the functions of the standard library to read from a file and exctract a string :

```C++
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
```

Add this in your .cpp, and add the declaration to the .hpp.
And now, let's change the constructor to take the filepath to the vertex and fragment shader instead of the source code directly.
Once you're done, change the construction of the pipeline in your main to use to shader files that are in the "res" folder and try to run it ! : it should work just as before, but the code is so much cleaner :heart: Well done !