# Debugging OpenGL

Through these exercices, you will improve your skills to catch OpenGL errors. You will understand how to detect and prevent them.

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

// Check compilation
int success;
glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(vs, 512, NULL, infoLog);
    spdlog::critical("[Shader] Compilation failed : {}", infoLog);
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

## 03 - Buffers everywhere

> Learn to use RenderDoc to Debug OpenGL buffers



## 04 - Uniforms in shape

> Updates uniforms in a safer way

## 05 - Cube, mesh, and the universe

> Display a cube in 3D and use RenderDoc to check for data inconsistensy
