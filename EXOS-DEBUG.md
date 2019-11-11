# Debugging OpenGL

Through these exercices, you will improve your skills to catch OpenGL errors. You will understand how to detect and prevent them.

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

- line 24, 25, 26 : GL_FRAMEBUFFER to GL_ARRAY_BUFFER 
- line 31 :  glGenVertexArrays(0, &vao); to  glGenVertexArrays(1, &vao);

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

- line 53, 77 : Shader version must be first line
- line 57 : missing ;
- line 82 : FragColor

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
- line 57: glEnableVertexAttribArray(0) to glEnableVertexAttribArray(1)
- line 59: glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(char), NULL) to glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL)

</details>

If, even with the correction, you can't make it, go check `cheat/debug-03/main.cpp`.

___

## 04 - Uniforms in shape

> Updates uniforms in a safer way

### 🤔 The problem



### 🧐 What is available ?



### 👌 The concrete solution


### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-04/*)` and open the `debug-04/main.cpp`.


<details><summary>Correction</summary>

There were X errors to fix :

</details>

If, even with the correction, you can't make it, go check `cheat/debug-04/main.cpp`.

___

## 05 - Cube, mesh, and the universe

> Display a cube in 3D and use RenderDoc to check for data inconsistensy

### 🤔 The problem


### 🧐 What is available ?


### 👌 The concrete solution


### 💪 The exercice

Modify the `CMakeLists.txt` at the line 12 with `file(GLOB_RECURSE MY_SOURCES src/debug-05/*)` and open the `debug-05/main.cpp`.


<details><summary>Correction</summary>

There were X errors to fix :

</details>

If, even with the correction, you can't make it, go check `cheat/debug-05/main.cpp`.
