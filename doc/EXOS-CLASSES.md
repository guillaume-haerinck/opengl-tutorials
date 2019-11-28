# Abstracting OpenGL objects

Moving on from the Debug exercises, which lead us to drawing a cube, we will now try to draw several of them with different shader-pipelines.

In order to do so without too much pain, we first need to put all that code into classes to be able to reuse it easily.

## Table of Contents

+ [ShaderPipeline class](#01---shaderpipeline-class)
+ [ShaderPipeline improvements !](#02---shaderpipeline-improvements)
+ [CubeMesh class](#03---cube-mesh)
+ [CubeMesh improvements !](#04---cubemesh-improvements)
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

We will use the functions of the standard library to read from a file and extract a string :

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

This function has been added to the class, now you have to change the constructor to take the filepath to the vertex and fragment shader instead of the source code directly.

Once you're done, change the construction of the pipeline in your main to use the shader files that are in the "res" folder and try to run it ! it should work just as before, but the code is so much cleaner :heart: Well done !


## 03 - CubeMesh

> Send the buffers to the GPU, and draw in one line !

Just as before, you've got an empty CubeMesh class to fill with code you'll find in your main ;)

## 04 - CubeMesh improvements

> Lots of cubes in one draw call !

To reuse our cube and tell OpenGL to draw it in many different places, we're gonna use something called "instancing".
The idea is very straight forward : we create a buffer containing all the positions where we want a cube to be drawn, send it to the GPU and tell OpenGL what this buffer is (a.k.a. vertexAttribPointer).
Then, our position will be accessible in the shader just as any other attribute !

In the CubeMesh.cpp file we've added a "GLuint m_vbTranslations" attribute that is the OpenGl ID for our new buffer. 

So first, you must create the buffer on the GPU.

<details><summary>Solution</summary>
Simply add this in the constructor of CubeMesh :
'''C++
GLCall(glGenBuffers(1, &m_vbTranslations));
'''

</details>

Then, specify the layout : the vertexAttribPointer as usual, plus one line :
'''C++
glVertexAttribDivisor([put_your_attribute_location_here], 1);
'''
This is to tell OpenGL that we're sending an instanced buffer and that the attribute should stay the same for all the vertices of each instance (instead of changing for each vertex, which is the defaut behaviour).

<details><summary>Solution</summary>
'''C++
GLCall(glEnableVertexAttribArray(1));
GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbTranslations));
GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));
GLCall(glVertexAttribDivisor(1, 1));
'''

</details>

Now you can go in your shader, add the corresponding attribute, and use it in the computation of "gl_Position" !

<details><summary>Solution</summary>

'''glsl
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTranslation;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main() {
    gl_Position = uViewProj * uModel * vec4(aPos + aTranslation, 1.0);
}
'''
</details>

Now we are almost ready to draw lot's of cubes ! But where ? How many ? It's time to fill the "addCube" function ! :

We need to keep the list on the GPU ("std::vector<glm::vec3> m_translations" is here for that purpose), and also dont forget to send the data to the GPU ! (we should resend it each time a cube is added to our list)

<details><summary>Solution</summary>

'''C++
void CubeMesh::addCube(const glm::vec3& translation) {
	m_translations.push_back(translation);
	// Update GPU
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbTranslations));
	GLCall(glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * m_translations.size(), &(m_translations[0]), GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
'''

</details>

And of course we must use it in the "main" to add a few cubes :

'''C++
CubeMesh cube;
cube.addCube(glm::vec3(0., 0., 1.));
cube.addCube(glm::vec3(4., 0., 1.));
cube.addCube(glm::vec3(-4., 0., 1.));
'''

Finally, the draw call will also be slightly different : it's now "glDrawElementsInstanced" instead of "glDrawElements". The parameters are slightly different too, and I suggest you take a look at the documentation to find out :wink:
If you don't find, here's the answer : 

<details><summary>Solution</summary>

You simply have to add a parameter to specify how many instances you want to draw :

'''C++
glDrawElementsInstanced(GL_TRIANGLES, std::size(squareData::indices), GL_UNSIGNED_SHORT, (void*)0, m_translations.size());
'''

</details>