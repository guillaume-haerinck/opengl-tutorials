#pragma once

#include <glad/glad.h>

class SquareVao {
public:
	SquareVao();
	~SquareVao();

	void draw();

private:
	GLuint m_iboID;
	GLuint m_vboPositionID;
	GLuint m_vboColorID;
	GLuint m_vaoID;
};