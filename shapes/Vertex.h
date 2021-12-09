#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include "GL/glew.h"

class Vertex
{
public:
    Vertex();
	Vertex(GLfloat xPos, GLfloat yPos, GLfloat zPos);
	Vertex(GLfloat xPos, GLfloat yPos, GLfloat zPos, glm::vec3 vertexNormals);
	Vertex(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xNormal, GLfloat yNormal, GLfloat zNormal);
    ~Vertex();

	glm::vec3 pos;
	glm::vec3 nor;
private: 
	// glm::vec3 defaultVertexNor = glm::vec3(0, 0, 0);
	glm::vec3 defaultVertexNor = glm::vec3(1, 0, 0);
};

#endif // VERTEX_H
