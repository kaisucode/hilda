#include "shapes/ConeShape.h"

ConeShape::ConeShape()
{
}

ConeShape::ConeShape(int param1, int param2) 
{
	m_param1 = std::max(1, param1); // number of layers of triangles
	m_param2 = std::max(3, param2); // number of sides
	
	// build circumference vertices (initialized with y-values of -0.5f)
	std::vector<Vertex> circumferenceVertices = this->buildCircumferenceVertices(m_param2);

	this->buildCap(circumferenceVertices, -0.5, false);
	this->buildSides(circumferenceVertices);

	initializeOpenGLShapeProperties();
}

void ConeShape::buildSides(std::vector<Vertex> circumferenceVertices) {
	Vertex coneTipVertex = Vertex(0, 0.5f, 0);
	glm::vec3 baseVertexNormal = glm::vec3(2/sqrt(5), 1/sqrt(5), 0);

	for (int numOfSides = 0; numOfSides < m_param2; numOfSides++) 

	{
		int nextColIdx = (numOfSides + 1) % m_param2;

		GLfloat thetaDelta = 2.0f * M_PI / m_param2;
		GLfloat theta = thetaDelta * numOfSides;
		GLfloat nextTheta = theta + thetaDelta;

		Vertex bottomLeftVertex = circumferenceVertices.at(numOfSides);
		Vertex bottomRightVertex = circumferenceVertices.at(numOfSides + 1);

		Vertex curLeftVertex = coneTipVertex;
		Vertex curRightVertex = coneTipVertex;

		coneTipVertex.nor = glm::normalize(glm::rotateY(baseVertexNormal, -theta - thetaDelta / 2.0f));
		curLeftVertex.nor = glm::normalize(glm::rotateY(baseVertexNormal, -theta));
		curRightVertex.nor = glm::normalize(glm::rotateY(baseVertexNormal, -nextTheta));

		buildCapSlice(bottomLeftVertex, bottomRightVertex, coneTipVertex, curLeftVertex, curRightVertex);
	}
}


ConeShape::~ConeShape()
{
}

