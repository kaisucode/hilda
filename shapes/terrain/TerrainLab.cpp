#include "shapes/terrain/TerrainLab.h"

TerrainLab::TerrainLab()
{
	TerrainBase();
}

TerrainLab::TerrainLab(int param1, int param2) 
{
	TerrainBase(param1, param2);
	for (int row = 0; row < m_numRows - 1; row++) {
		for (int col = m_numCols - 1; col >= 0; col--) {
			insertVec3(m_vertexData, getPosition(row, col));
			insertVec3(m_vertexData, getNormal(row, col));
			insertVec3(m_vertexData, getPosition(row + 1, col));
			insertVec3(m_vertexData, getNormal(row + 1, col));
		}
		insertVec3(m_vertexData, getPosition(row + 1, 0));
		insertVec3(m_vertexData, getNormal(row + 1, 0));
		insertVec3(m_vertexData, getPosition(row + 1, m_numCols - 1));
		insertVec3(m_vertexData, getNormal(row + 1, m_numCols - 1));
	}

	initializeOpenGLShapeProperties();
}



/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and
 * column.
 */
float TerrainLab::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}

float TerrainLab::getNoise(glm::vec3 x)
{
	return 0.0f;
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */
glm::vec3 TerrainLab::getPosition(int row, int col)
{
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
	
	int newRowFloor = glm::floor(row / 20.0f);
	int newColFloor = glm::floor(col / 20.0f);
	int newRowCeiling = glm::ceil(row / 20.0f);
	int newColCeiling = glm::ceil(col / 20.0f);

	float aYPosition = randValue(newRowFloor, newColFloor);
	float bYPosition = randValue(newRowFloor, newColCeiling);
	float cYPosition = randValue(newRowCeiling, newColFloor);
	float dYPosition = randValue(newRowCeiling, newColCeiling);

	float u = glm::fract(col / 20.0f);
	float v = glm::fract(row / 20.0f);

	float bicubicU = pow(u, 2) * (3 - 2 * u);
	float bicubicV = pow(v, 2) * (3 - 2 * v);

	float ab = glm::mix(aYPosition, bYPosition, bicubicU);
	float cd = glm::mix(cYPosition, dYPosition, bicubicU);

	position.y = glm::mix(ab, cd, bicubicV) / m_length_modifier;

	int newYRowCeiling = glm::ceil(row / 20.0f);
	int newYColCeiling = glm::ceil(col / 20.0f);

	position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.05;
	return position;

}


TerrainLab::~TerrainLab()
{
}

