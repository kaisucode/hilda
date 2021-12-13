#include "shapes/terrain/TerrainBowl.h"

TerrainBowl::TerrainBowl()
{
	TerrainBase();
}

TerrainBowl::TerrainBowl(int param1, int param2) 
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
float TerrainBowl::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}

float TerrainBowl::getNoise(glm::vec3 x)
{
	return 0.0f;
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */
glm::vec3 TerrainBowl::getPosition(int row, int col)
{
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
	
	// position.y = randValue(row, col);

	int newRowFloor = glm::floor(row / 20.0f);
	int newColFloor = glm::floor(col / 20.0f);
	int newRowCeiling = glm::ceil(row / 20.0f);
	int newColCeiling = glm::ceil(col / 20.0f);
	// position.y = randValue(newRow, newCol);

	float aYPosition = randValue(newRowFloor, newColFloor);
	float bYPosition = randValue(newRowFloor, newColCeiling);
	float cYPosition = randValue(newRowCeiling, newColFloor);
	float dYPosition = randValue(newRowCeiling, newColCeiling);

	// float aYPosition = randValue(glm::floor(row / 20.0f), glm::floor(newCol);
	// float bYPosition = randValue(newRow, newCol + 1);
	// float cYPosition = randValue(newRow + 1, newCol);
	// float dYPosition = randValue(newRow + 1, newCol + 1);


	// u = (position.x - row * 20.0f) / 20.0f;
	// v = (position.z - col * 20.0f) / 20.0f;

	float u = glm::fract(col / 20.0f);
	float v = glm::fract(row / 20.0f);

	// float bicubicU = pow(u, 2) * (3 - 2 * u);
	// float bicubicV = pow(v, 2) * (3 - 2 * v);

	float bicubicU = pow(u, 2) * (3 - 2 * u);
	float bicubicV = pow(v, 2) * (3 - 2 * v);

	float ab = glm::mix(aYPosition, bYPosition, bicubicU);
	float cd = glm::mix(cYPosition, dYPosition, bicubicU);

	position.y = glm::mix(ab, cd, bicubicV) / m_length_modifier;

	int newYRowCeiling = glm::ceil(row / 20.0f);
	int newYColCeiling = glm::ceil(col / 20.0f);

	float distFromCenterSquared = position.x * position.x + position.z * position.z;

	float noise = 0.08 / (m_length_modifier * m_length_modifier);
	// float noise = 0.08 / (m_length_modifier);

	if (distFromCenterSquared < centerRadius * centerRadius) {
		// position.y -= 5;
		position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * noise / 2;
		// position.y -= fabs(m_numRows - distFromCenterSquared) * 0.03 - 1;
		// position.y -= fabs(m_numRows - distFromCenterSquared) * 0.01;
    // position.x = 10 * row/m_numRows - 5;
		position.y -= fabs(10 + 5 - distFromCenterSquared) * 0.25;
	}
	else if (distFromCenterSquared < centerRadius * centerRadius) {
		// position.y -= 5;
		position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * noise / 2;
		// position.y -= fabs(m_numRows - distFromCenterSquared) * 0.03 - 1;
		// position.y -= fabs(m_numRows - distFromCenterSquared) * 0.01;
    // position.x = 10 * row/m_numRows - 5;
		position.y -= fabs(10 + 5 - distFromCenterSquared) * 0.25;
	}
	else {
		position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * noise;
	}
	
	// position.y /= m_length_modifier;
	// position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08;
	// position.y = position.y * newYColCeiling + randValue(newYRowCeiling, newYColCeiling);
    // position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08 + 0.3 * randValue(newYRowCeiling, newYColCeiling);
	// position.y = randValue(newRow, newCol);

    return position;
}


TerrainBowl::~TerrainBowl()
{
}

