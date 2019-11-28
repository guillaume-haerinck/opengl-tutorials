#include "CubeMesh.hpp"

#include "common/gl-exception.h"
#include "common/square-data.h"
#include <iterator>

CubeMesh::CubeMesh(){
	// ------------------ Vertex Buffer 1
	{
		GLCall(glGenBuffers(1, &m_vbPos));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbPos));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(squareData::positions), squareData::positions, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	// (1) we must generate a buffer on the GPU for m_vbTranslations

	// ------------------ Vertex Array
	{
		GLCall(glGenVertexArrays(1, &m_vao));
		GLCall(glBindVertexArray(m_vao));

		// Vertex input description
		{
			GLCall(glEnableVertexAttribArray(0));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbPos));
			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));
		}
		
		// (2) and specify the vertexAttribPointer for m_vbTranslations

		GLCall(glBindVertexArray(0));
	}

	// ------------------ Index buffer
	{
		GLCall(glGenBuffers(1, &m_ib));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareData::indices), squareData::indices, GL_STATIC_DRAW));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}

CubeMesh::~CubeMesh() {
	GLCall(glDeleteBuffers(1, &m_vbPos));
	// (3) delete m_vbTranslations
	GLCall(glDeleteBuffers(1, &m_ib));
	GLCall(glDeleteVertexArrays(1, &m_vao));
}

void CubeMesh::addCube(const glm::vec3& translation) {
	// (4) do *something* here ;)
}

void CubeMesh::draw() {
	GLCall(glBindVertexArray(m_vao));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib));
	// (5) the drawCall will be slightly different for instanced rendering, so modify the next line : 
	GLCall(glDrawElements(GL_TRIANGLES, std::size(squareData::indices), GL_UNSIGNED_SHORT, (void*)0));
}