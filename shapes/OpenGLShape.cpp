#include "OpenGLShape.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"

using namespace CS123::GL;

OpenGLShape::OpenGLShape() :
    m_VAO(nullptr)
{

}

OpenGLShape::~OpenGLShape()
{
}

void OpenGLShape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void OpenGLShape::initializeOpenGLShapeProperties() {
    const int numFloatsPerVertex = 6;
    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}


void OpenGLShape::buildCap(std::vector<Vertex> circumferenceVertices, GLfloat yValue, bool orientationIsUp) 
{

	glm::vec3 topNormalVector = glm::vec3(0.f, 1.0f, 0.f);
	glm::vec3 bottomNormalVector = glm::vec3(0.f, -1.0f, 0.f);

	glm::vec3 normalVector = (orientationIsUp) ? topNormalVector : bottomNormalVector;
	Vertex centerVertex = Vertex(0, yValue, 0, normalVector);

	for (int numOfSides = 0; numOfSides < m_param2; numOfSides++) {
		Vertex bottomLeftVertex = circumferenceVertices.at(numOfSides);
		Vertex bottomRightVertex = circumferenceVertices.at(numOfSides + 1);

		bottomLeftVertex.pos.y = yValue;
		bottomRightVertex.pos.y = yValue;

		buildCapSlice(bottomLeftVertex, bottomRightVertex, centerVertex, centerVertex, centerVertex, orientationIsUp);
	}

}

void OpenGLShape::buildCapSlice(Vertex bottomLeftVertex, Vertex bottomRightVertex,\
		Vertex centerVertex, Vertex curLeftVertex, Vertex curRightVertex, bool orientationIsUp)
{
	std::vector<std::vector<Vertex>> vertexInfo;
	vertexInfo.reserve(m_param1 * 2);

	glm::vec3 leftDiff = (bottomLeftVertex.pos - centerVertex.pos) / (m_param1 * 1.0f);
	glm::vec3 rightDiff = (bottomRightVertex.pos - centerVertex.pos) / (m_param1 * 1.0f);

	for (int i = 0; i < m_param1; i++) {
		curLeftVertex.pos += leftDiff;
		curRightVertex.pos += rightDiff;
		vertexInfo.push_back({ curLeftVertex, curRightVertex });
	}

	for (int layer = 0; layer < m_param1; layer++) {

		Vertex p1, p2, p3, p4;

		if (layer == 0) { // use center vertex to create one triangle
			p1 = vertexInfo.at(0).at(0);
			p2 = centerVertex;
			p3 = vertexInfo.at(0).at(1);

			TriangleShape aTriangle = TriangleShape(p1, p2, p3);
			addTriToVertexData(aTriangle, !orientationIsUp);
		}
		else { // create two triangles
			p1 = vertexInfo.at(layer - 1).at(0);
			p2 = vertexInfo.at(layer - 1).at(1);
			p3 = vertexInfo.at(layer).at(0);
			p4 = vertexInfo.at(layer).at(1);

			TriangleShape topTriangle = TriangleShape(p1, p2, p4);
			TriangleShape bottomTriangle = TriangleShape(p4, p3, p1);
			addTriToVertexData(topTriangle, !orientationIsUp);	
			addTriToVertexData(bottomTriangle, !orientationIsUp);
		}

	}
}

std::vector<Vertex>	OpenGLShape::buildCircumferenceVertices(int numOfVertices) 
{
	std::vector<Vertex> circumferenceVertices;
	circumferenceVertices.reserve(numOfVertices + 1);

	for (int numOfSides = 0; numOfSides <= numOfVertices; numOfSides++) 
	{
		GLfloat theta = 2 * M_PI * (numOfSides % numOfVertices) / numOfVertices;

		glm::vec3 aVertexNormal = glm::normalize(glm::vec3(glm::cos(theta), 0, glm::sin(theta)));
		Vertex aVertex = Vertex(0.5f * glm::cos(theta), -m_radius, 0.5f * glm::sin(theta), aVertexNormal);

		circumferenceVertices.push_back(aVertex);
	}
	return circumferenceVertices;
}

void OpenGLShape::addTriToVertexData(TriangleShape aTriangle, bool shouldReverse) 
{
	if (shouldReverse) {
		for (int i = 2; i >= 0; i--) {
			insertVec3(m_vertexData, aTriangle.v.at(i).pos);
			insertVec3(m_vertexData, aTriangle.v.at(i).nor);
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			insertVec3(m_vertexData, aTriangle.v.at(i).pos);
			insertVec3(m_vertexData, aTriangle.v.at(i).nor);
		}
	}
}

void OpenGLShape::addGridToVertex(std::vector<std::vector<Vertex>> vertexInfo,\
		int maxRows, int maxCols, bool shouldReverse) 
{
	for (int rowIdx = 0; rowIdx < maxRows; rowIdx++) {
		for (int colIdx = 0; colIdx < maxCols; colIdx++) {
			Vertex p1, p2, p3;

			p1 = vertexInfo.at(rowIdx).at(colIdx);
			p2 = vertexInfo.at(rowIdx + 1).at(colIdx);
			p3 = vertexInfo.at(rowIdx + 1).at(colIdx + 1);
			TriangleShape firstTriangle = TriangleShape(p1, p2, p3);
			addTriToVertexData(firstTriangle, shouldReverse);

			p1 = vertexInfo.at(rowIdx + 1).at(colIdx + 1);
			p2 = vertexInfo.at(rowIdx).at(colIdx + 1);
			p3 = vertexInfo.at(rowIdx).at(colIdx);
			TriangleShape secondTriangle = TriangleShape(p1, p2, p3);
			addTriToVertexData(secondTriangle, shouldReverse);
		}
	}
}

void OpenGLShape::setShapeType(int newShapeType) 
{
	this->shapeType = newShapeType;
	return;
}

