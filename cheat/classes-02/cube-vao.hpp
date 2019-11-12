#pragma once

#include <glad/glad.h>

class CubeVAO {
public:
	CubeVAO();
	~CubeVAO();

	void draw();

private:
	GLuint m_iboID;
	GLuint m_vboPositionID;
	GLuint m_vboNormalID;
	GLuint m_vaoID;
};