#include "shapes/SphereShape.h"

SphereShape::SphereShape()
{
}

SphereShape::SphereShape(int param1, int param2) 
{
	m_param1 = std::max(2, param1);
	m_param2 = std::max(3, param2);
	
	this->buildSides();

	initializeOpenGLShapeProperties();
}

void SphereShape::buildSides() {

	std::vector<std::vector<Vertex>> vertexInfo;
	vertexInfo.reserve((m_param1 + 1) * 2);

	for (int colIdx = 0; colIdx < m_param2; colIdx++) {
		vertexInfo.clear();

		int nextColIdx = (colIdx + 1) % m_param2;
		GLfloat theta = 2.0f * M_PI * colIdx / m_param2;
		GLfloat nextTheta = 2.0f * M_PI * nextColIdx / m_param2;

		for (int rowIdx = 0; rowIdx < m_param1; rowIdx++) {
			GLfloat phi = M_PI * rowIdx / m_param1;

			Vertex leftVertex = getSphereCoordinates(phi, theta);
			Vertex rightVertex = getSphereCoordinates(phi, nextTheta);

			leftVertex.nor = glm::normalize(leftVertex.pos);
			rightVertex.nor = glm::normalize(rightVertex.pos);

			vertexInfo.push_back({ leftVertex, rightVertex });
		}
		
		Vertex bottomCap = Vertex(0, -0.5f, 0);
		bottomCap.nor = bottomCap.pos;
		vertexInfo.push_back( {bottomCap, bottomCap} );

		addGridToVertex(vertexInfo, m_param1, 1, true);
	}
	return;

}

Vertex SphereShape::getSphereCoordinates(GLfloat phi, GLfloat theta) 
{
	return Vertex(this->m_radius * glm::sin(phi) * glm::cos(theta),\
			this->m_radius * glm::cos(phi),\
			this->m_radius * glm::sin(phi) * glm::sin(theta));
}

SphereShape::~SphereShape()
{
}

