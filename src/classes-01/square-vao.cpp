#include "square-vao.hpp"
#include "common/gl-exception.h"

SquareVao::SquareVao() {
	// Generate buffers
	GLCall(glGenBuffers(1, &m_vboPositionID));
	GLCall(glGenBuffers(1, &m_vboColorID));
	GLCall(glGenVertexArrays(1, &m_vaoID));

	// Create index buffer (a.k.a. ibo)
	GLCall(glGenBuffers(1, &m_iboID));

	unsigned int iboData[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iboData), iboData, GL_STATIC_DRAW));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	// Send data and Specify vertex attribute ( =layout) (openGL will store everything in the vao)
	GLCall(glBindVertexArray(m_vaoID));
		// Positions
	float vboPositionData[] = {
		-0.5, -0.5,
		-0.5,  0.5,
		 0.5,  0.5,
		 0.5, -0.5
	};
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboPositionID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vboPositionData), vboPositionData, GL_STATIC_DRAW));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

SquareVao::~SquareVao() {
	glDeleteBuffers(1, &m_iboID);
	glDeleteBuffers(1, &m_vboPositionID);
	glDeleteBuffers(1, &m_vboColorID);
	glDeleteVertexArrays(1, &m_vaoID);
}

void SquareVao::draw() {
	// Bind
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
	GLCall(glBindVertexArray(m_vaoID));
	// Draw
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	// Unbind
	GLCall(glBindVertexArray(0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}