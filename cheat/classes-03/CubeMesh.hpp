#pragma once

#include <glad/glad.h> // OpenGL

class CubeMesh {
public:
	CubeMesh();
	~CubeMesh();

	void draw();

private:
	GLuint m_ib;
	GLuint m_vbPos;
	GLuint m_vao;
};