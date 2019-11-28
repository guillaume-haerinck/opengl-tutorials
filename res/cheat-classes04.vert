#version 330 core

//Cubes rotating on themselves :
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTranslation;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main() {
    gl_Position = uViewProj * uModel * vec4(aPos + aTranslation, 1.0);
}

// Cubes rotating around the origin :
/*layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTranslation;

uniform mat4 uModel;
uniform mat4 uViewProj;

void main() {
	vec4 pos = uModel * vec4(aPos, 1.0);
	pos += vec4(aTranslation, 1.);
    gl_Position = uViewProj * pos;
}*/

