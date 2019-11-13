# Debugging OpenGL

Through these exercices, you will improve your skills to catch OpenGL errors. You will understand how to detect and prevent them.

## Table of Contents

+ [Catch OpenGL API errors](#01---call-me-maybe)
+ [Catch Shader errors](#02---50-shades-of-errors)
+ [Debug buffer data](#03---buffers-everywhere)
+ [Uniforms handling](#04---uniforms-in-shape)
+ [Everything together](#05---cube-mesh-and-the-universe)

___

## 01 - Call me maybe

> Learn to get OpenGL errors when calling functions

### 🤔 The problem

If you made a mistake when calling OpenGL, you won't know why and when by default.

### 🧐 What is available ?

OpenGL does generate error messages if the paramaters of a function call do not match the ones allowed by OpenGL, or if a function does not interact reasonably with the currently bound context.

These errors are stored in a queue until they are handled by the developer. 

This means that if you do not check for errors between each OpenGL function call, then you won't know from which function the error came from.

### 👌 The concrete solution

Wrap each call made to OpenGL inside a macro which will ask OpenGL for errors. If there is any, trigger a breakpoint and output the line and the error on the command line.

```C++
#include <debug_break/debug_break.h>

#define BreakAssert(x) if (!x) { debug_break(); assert(false); }

#define GLCall(x) glexp::clear(); x; BreakAssert(glexp::doesFunctionWorks(#x, __FILE__, __LINE__))

namespace glexp {
    /**
     * @brief Empty the OpenGl error buffer
     */
    void clear();

    /**
     * @brief Print OpenGl errors to the console if any
     * 
     * @param functionName
     * @param filename
     * @param line
     */
    bool doesFunctionWorks(const char* functionName, const char* filename, int line);
}

(...)

GLCall(glXXX(myParams));
```

### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-01/*)`

Then open the `debug-01/main.cpp` and include ` "common/gl-exception.h"`. 

Now you can wrap you OpenGL calls with GLCall, and debug the errors in the file. Try it by yourself. If you get stuck, the correction is below.

<details><summary>Correction</summary>

There were 2 mistakes to find :

- `line 24, 25, 26` : GL_FRAMEBUFFER to GL_ARRAY_BUFFER 
- `line 31` :  glGenVertexArrays(0, &vao); to  glGenVertexArrays(1, &vao);

</details>

If, even with the correction, you can't make it, go check `cheat/debug-01/main.cpp`.

___

## 02 - 50 shades of errors

> Learn to get OpenGL shader building errors

### 🤔 The problem

If you made a mistake in your glsl shader source files, you won't know what and where by default.

### 🧐 What is available ?

You can ask OpenGL if the shader compilation succeeded, and if not, you can get the precise compilation errors.

### 👌 The concrete solution

```C++
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

unsigned int vs = glCreateShader(GL_VERTEX_SHADER);

(...)

// Check compilation
int success;
glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    spdlog::critical("[VertexShader] Compilation failed : {}", infoLog);
    debug_break();
}
```

It is almost the same when linking shaders in a Program.

### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-02/*)` and open the `debug-02/main.cpp`.

Then check errors with the vertex shader and the fragment shader compilation, and fix them.

<details><summary>Correction</summary>

There were 3 errors to fix :

- `line 53, 77` : Shader version must be first line
- `line 57` : missing ;
- `line 82` : FragColor

</details>

If, even with the correction, you can't make it, go check `cheat/debug-02/main.cpp`.

___

## 03 - Buffers everywhere

> Learn to use RenderDoc to Debug OpenGL buffers

### 🤔 The problem

If your vertex buffer is corrupted, OpenGL won't help you much to find why and how.

### 🧐 What is available ?

Graphic debugger are third-party softwares made to catch and analyze calls made to graphic APIs like OpenGL. Equiped with it, you can follow what is happening at each frame with your software. One of the well-known is called [RenderDoc](https://renderdoc.org/). It's free, open-source and we are going to use it to debug our app.

### 👌 The concrete solution

At the first opening, this software can be a bit scary. Don't worry, you will get used to it by end of this session. You need to launch you app with it, and capture a frame with `f12` to analyze it. Try it with your last build and play around with it a bit.

### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-03/*)` and open the `debug-03/main.cpp`. Build and launch.

Strange right ? There should be a colored triangle, but nothing is printed on screen and no errors are reported by OpenGL ! There is only one way to know what happens, Open [RenderDoc](https://renderdoc.org/), launch and analyse the buffer data !

Then have a look on the Vertex input description in your source file and try to fix the problem.

<details><summary>Correction</summary>

There were 2 errors to fix :
- `line 57` : glEnableVertexAttribArray(0) to glEnableVertexAttribArray(1)
- `line 59` : glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(char), NULL) to glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL)

</details>

If, even with the correction, you can't make it, go check `cheat/debug-03/main.cpp`.

___

## 04 - Uniforms in shape

> Updates uniforms in a safer way

### 🤔 The problem

To updates the uniforms you need to know their names with string, which is error-prone.

### 🧐 What is available ?

OpenGL returns -1 with glGetUniformLocation if the uniform does not exist. We need to handle this result.

### 👌 The concrete solution

Asking for the location of the uniform each time that we need to update it is ineficcient. Instead, let's store this result and reuse it each time that we need it :

```C++

#include <string>
#include <unordered_map>

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
```

### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-04/*)` and open the `debug-04/main.cpp`.

Nothing on screen ! Time to check RenderDoc to see where this might come from. Then go back to the source code and check if the uniform does exist.

<details><summary>Correction</summary>

There were 1 error to fix :

- `line 152` : model to uModel

</details>

If, even with the correction, you can't make it, go check `cheat/debug-04/main.cpp`.

___

## 05 - Cube, mesh, and the universe

> Display a cube in 3D and use RenderDoc to check for data inconsistency

### 🤔 The problem

Some problems can make your program crash before you open it. We're going to handle this case with everything we've done before.

### 🧐 What is available ?

Everything that we've seen before ! This is a best-of the worst.

### 👌 The concrete solution

Proceed step by step. At first, prevent your program from crashing by setting up breakpoints and commenting-out draw calls. Then analyse your data with RenderDoc and fix it progressively.

### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-05/*)` and open the `debug-05/main.cpp`.

When there is an execution problem, start by commenting out the draw call. If it launches, go check the buffers with RenderDoc.

<details><summary>Step 01 Correction</summary>
Comment out the line 190 and launch the program with RenderDoc.

If you check the buffer in the Pipeline State, you will see that it has a ByteLength of 0. It is empty.

<p align="center">
<img src="img/empty-vertex-buffer.png" alt="RenderDoc screenshot">
</p>

Go check line 40 and fix the buffer allocation data.
</details>

Great ! Now you have your data, but there is still nothing on screen. Time for another round with render doc to check if what contains the Vertex Buffer.

<details><summary>Step 02 Correction</summary>
If you check the buffer data, you will see that it is not lisible, even though it is not empty. This means that we haven't properly said to OpenGL how the data is laid out.

<p align="center">
<img src="img/cannot-read-vertex-buffer.png" alt="RenderDoc screenshot">
</p>

Go check line 54 and fix the layout.
</details>

Almost there, but still nothing on screen. If we launch RenderDoc again, we can check that the indexed-draw does have indices.

<details><summary>Step 03 Correction</summary>
If you check the buffer data, you will see that the indices are always 0.

<p align="center">
<img src="img/null-index-buffer.png" alt="RenderDoc screenshot">
</p>

Go check line 65 and ensure that some data is sent.
</details>

Woo there is a flat cube ! But why is it flat ? Check your uniforms see if they are updated correctly.

<details><summary>Correction</summary>

There were 4 errors to fix :

- `line 40` : glBufferData(GL_ARRAY_BUFFER, 0, squareData::positions, GL_STATIC_DRAW) to glBufferData(GL_ARRAY_BUFFER, sizeof(squareData::positions), squareData::positions, GL_STATIC_DRAW)
- `line 54` : glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3, NULL) to glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL)
- `line 65` : glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareData::indices), 0, GL_STATIC_DRAW) to glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareData::indices), squareData::indices, GL_STATIC_DRAW)
- `line 184` : &modelMat[0][0] to &viewProjMat[0][0]

</details>

If, even with the correction, you can't make it, go check `cheat/debug-05/main.cpp`.
