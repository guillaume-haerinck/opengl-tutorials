# Abstracting OpenGL objects

Moving on from the Debug exercises, which lead us to drawing a cube, we will now try to draw several of them with different shader-pipelines.

In order to do so without too much pain, we first need to put all that code into classes to be able to reuse it easily.

## Table of Contents

+ [ShaderPipeline class](#01---shaderpipeline-class)
+ [ShaderPipeline improvements !](#02---shaderpipeline-improvements)
+ [Debug buffer data](#03---buffers-everywhere)
+ [Uniforms handling](#04---uniforms-in-shape)
+ [Everything together](#05---cube-mesh-and-the-universe)

___

## 01 - ShaderPipeline class

> Compile shaders into a pipepline and bind uniforms, all in one line !

Take a look at the ShaderPipeline.hpp file in "classes-01" : there's quite a lot of methods to implement ! 

All the code you need is already in the main.cpp ! Just look around, find the bits of code that involve the shader pipeline, and move them into ShaderPipeline.cpp :)

Don't forget to also remove the code from the main, and replace it by the appropriate method call ;)


## 02 - ShaderPipeline improvements

> Read shaders from a file instead of writting it as a string in the .cpp

#### 🤔 The problem

It's **** illisible, you have no syntax highlight, nothing :weary: and the code cannot be reused for several shaders.

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

Once you're done, change the construction of the pipeline in your main to use to shader files that are in the "res" folder and try to run it ! it should work just as before, but the code is so much cleaner :heart: Well done !