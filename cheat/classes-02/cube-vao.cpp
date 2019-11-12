#include "cube-vao.hpp"
#include "common/gl-exception.h"

CubeVAO::CubeVAO() {
	// Generate buffers
	GLCall(glGenBuffers(1, &m_iboID));
	GLCall(glGenBuffers(1, &m_vboPositionID));
	GLCall(glGenBuffers(1, &m_vboNormalID));
	GLCall(glGenVertexArrays(1, &m_vaoID));

	// Create index buffer (a.k.a. ibo)
	unsigned int iboData[] = {
		// Top Face
		0, 1, 2,
		0, 2, 3,
		// Bot Face
		4, 5, 6,
		4, 6, 7,
		// Front Face
		//8, 9, 10,
		//8, 10, 11,
		// Back Face
		12, 13, 14,
		12, 14, 15,
		// Right Face
		16, 17, 18,
		16, 18, 19,
		// Left Face
		20, 21, 22,
		20, 22, 23
	};
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iboData), iboData, GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Send data and Specify vertex attribute ( =layout) (openGL will store everything in the vao)
		// Bind vao
	GLCall(glBindVertexArray(m_vaoID));
		// Positions
	float vboPositionData[] = {
		// Top Face
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		// Bot Face
		-0.5, -0.5,  0.5,
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,
		// Front Face
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5, -0.5,  0.5,
		-0.5, -0.5,  0.5,
		// Back Face
		-0.5,  0.5, -0.5,
		 0.5,  0.5, -0.5,
		 0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,
		// Right Face
		 0.5, -0.5,  0.5,
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		 0.5, -0.5, -0.5,
		// Left Face
		-0.5, -0.5,  0.5,
		-0.5,  0.5,  0.5,
		-0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5
	};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboPositionID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vboPositionData), vboPositionData, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		// Normals
	float vboNormalData[] = {
		// Top Face
		 0.0,  1.0,  0.0,
		 0.0,  1.0,  0.0,
		 0.0,  1.0,  0.0,
		 0.0,  1.0,  0.0,
		// Bot Face
		 0.0, -1.0,  0.0,
		 0.0, -1.0,  0.0,
		 0.0, -1.0,  0.0,
		 0.0, -1.0,  0.0,
		// Front Face
		 0.0,  0.0,  1.0,
		 0.0,  0.0,  1.0,
		 0.0,  0.0,  1.0,
		 0.0,  0.0,  1.0,
		// Back Face
		 0.0,  0.0, -1.0,
		 0.0,  0.0, -1.0,
		 0.0,  0.0, -1.0,
		 0.0,  0.0, -1.0,
		// Right Face
		 1.0,  0.0,  0.0,
		 1.0,  0.0,  0.0,
		 1.0,  0.0,  0.0,
		 1.0,  0.0,  0.0,
		 // Left Face
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
		-1.0,  0.0,  0.0,
	};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboNormalID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vboNormalData), vboNormalData, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		// Unbind vao
	GLCall(glBindVertexArray(0));
}

CubeVAO::~CubeVAO() {
	glDeleteBuffers(1, &m_iboID);
	glDeleteBuffers(1, &m_vboPositionID);
	glDeleteVertexArrays(1, &m_vaoID);
}

void CubeVAO::draw() {
	// Bind
	GLCall(glBindVertexArray(m_vaoID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
	// Draw
	GLCall(glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0));
	// Unbind
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}