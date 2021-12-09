#include "shapes/CylinderShape.h"

CylinderShape::CylinderShape()
{
}

CylinderShape::CylinderShape(int param1, int param2) 
{
	m_param1 = std::max(1, param1); // number of layers of triangles
	m_param2 = std::max(3, param2); // number of sides
	
	// build circumference vertices (initialized with y-values of -0.5f)
	std::vector<Vertex> circumferenceVertices = this->buildCircumferenceVertices(m_param2);

	this->buildCap(circumferenceVertices, 0.5, true);
	this->buildCap(circumferenceVertices, -0.5, false);
	this->buildSides(circumferenceVertices);

	initializeOpenGLShapeProperties();
}

void CylinderShape::buildSides(std::vector<Vertex> circumferenceVertices) {
	std::vector<std::vector<Vertex>> vertexInfo;
	vertexInfo.reserve((m_param2 + 1) * 2);
	for (int numOfSides = 0; numOfSides < m_param2; numOfSides++) 
	{
		vertexInfo.clear();

		Vertex bottomLeftVertex = circumferenceVertices.at(numOfSides);
		Vertex bottomRightVertex = circumferenceVertices.at(numOfSides + 1);

		Vertex leftVertex = bottomLeftVertex;
		Vertex rightVertex = bottomRightVertex;
		GLfloat yPosDelta = (1.0f) / m_param1;

		for (int i = 0; i <= m_param1; i++) {
			vertexInfo.push_back({ leftVertex, rightVertex });
			leftVertex.pos.y += yPosDelta;
			rightVertex.pos.y += yPosDelta;
		}

		addGridToVertex(vertexInfo, m_param1, 1);
	}
}


CylinderShape::~CylinderShape()
{
}

