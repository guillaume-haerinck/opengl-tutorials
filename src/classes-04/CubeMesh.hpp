#pragma once

#include <glad/glad.h> // OpenGL
#include <vector>
#include "glm/glm.hpp"

class CubeMesh {
public:
	CubeMesh();
	~CubeMesh();

	void addCube(const glm::vec3& translation);
	void draw();

private:
	GLuint m_ib;
	GLuint m_vbPos;
	GLuint m_vao;

	std::vector<glm::vec3> m_translations;
	GLuint m_vbTranslations;
};