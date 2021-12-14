#include "shapes/terrain/TerrainBase.h"

TerrainBase::TerrainBase() :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
    m_triangleLayout = CS123::GL::VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP;
}

TerrainBase::TerrainBase(int param1, int param2) :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
    m_triangleLayout = CS123::GL::VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP;
	m_param1 = std::max(1, param1); // number of layers of triangles
	m_param2 = std::max(3, param2); // number of sides

	int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);
	m_vertexData.reserve(2 * numVertices * 3);
}


int TerrainBase::getVertexDataSize() {
    return m_vertexData.size();
}

glm::vec3 TerrainBase::getVertexAtIndex(int index) {
   GLfloat x = m_vertexData.at(index);
   GLfloat y = m_vertexData.at(index+1);
   GLfloat z = m_vertexData.at(index+2);
   return {x,y,z};
}


/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and
 * column.
 */
float TerrainBase::randValue(int row, int col)
{
	return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}

float TerrainBase::getNoise( glm::vec3 x )
{
	return 0.0f;
}


/**
 * Returns the normal vector for the terrain vertex at the given row and
 * column.
 */
glm::vec3 TerrainBase::getNormal(int row, int col)
{
    // TODO: Compute the normal at the given row and column using the positions
    //       of the neighboring vertices.

	int offsets[8][2]{ 
		{-1, -1}, {0, -1}, {1, -1}, 
		{1, 0}, {1, 1}, 
		{0,1}, {-1, 1}, {-1, 0}
	};

	glm::vec3 sumVector = glm::vec3(0, 0, 0);
	glm::vec3 p = getPosition(row, col);
	glm::vec3 lastVector = getPosition(row - 1, col);
	for (int i = 0; i < 8; i++) {
		glm::vec3 curVector = getPosition(row + offsets[i][0], col + offsets[i][1]);
		sumVector += glm::normalize(glm::cross(p - curVector, p - lastVector));
		lastVector = curVector;
	}

	return sumVector / 8.0f;
}

bool TerrainBase::isFilledIn() {
    return m_isFilledIn;
}


TerrainBase::~TerrainBase()
{
}

