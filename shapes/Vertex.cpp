#include "Vertex.h"


Vertex::Vertex()
{
	this->pos = glm::vec3(0, 0, 0);
	this->nor = this->defaultVertexNor;
}

Vertex::Vertex(GLfloat xPos, GLfloat yPos, GLfloat zPos)
{
	this->pos = glm::vec3(xPos, yPos, zPos);
	this->nor = this->defaultVertexNor;
}

Vertex::Vertex(GLfloat xPos, GLfloat yPos, GLfloat zPos, glm::vec3 vertexNormals)
{
	this->pos = glm::vec3(xPos, yPos, zPos);
	this->nor = vertexNormals;
}

Vertex::Vertex(GLfloat xPos, GLfloat yPos, GLfloat zPos,\
		GLfloat xNormal, GLfloat yNormal, GLfloat zNormal)
{
	this->pos = glm::vec3(xPos, yPos, zPos);
	this->nor = glm::vec3(xNormal, yNormal, zNormal);
}

Vertex::~Vertex()
{
}

