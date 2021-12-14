#include "shapes/terrain/TerrainCliff.h"

TerrainCliff::TerrainCliff()
{
	TerrainBase();
}

TerrainCliff::TerrainCliff(int param1, int param2)
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
float TerrainCliff::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


float TerrainCliff::hash(glm::vec3 p)  // replace this by something better
{
    p  = 50.0f * glm::fract(p * 0.3183099f + glm::vec3(0.71,0.113,0.419));
	return -1.0 + 2.0 * glm::fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}


float TerrainCliff::noised(glm::vec3 x)
{
	glm::vec3 i = glm::floor(x / 20.0f);
	glm::vec3 w = glm::fract(x / 20.0f);
    
    // quintic interpolation
	glm::vec3 u = w * w * w * (w * (w * 6.0f - 15.0f) + 10.0f);
	// glm::vec3 u = w * w * (3.0f - 2.0f * w);
	glm::vec3 du = 30.0f*w*w*(w*(w-2.0f)+1.0f);
    
	float temp = 20.0f;
    float a = hash(i + glm::vec3(0.0,0.0,0.0));
    float b = hash(i + glm::vec3(temp,0.0,0.0));
    float c = hash(i + glm::vec3(0.0,temp,0.0));
    float d = hash(i + glm::vec3(temp,temp,0.0));
    float e = hash(i + glm::vec3(0.0,0.0,temp));
	float f = hash(i + glm::vec3(temp,0.0,temp));
    float g = hash(i + glm::vec3(0.0,temp,temp));
    float h = hash(i + glm::vec3(temp, temp, temp));
	
    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

	return k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z;

}

float TerrainCliff::getNoise( glm::vec3 x )
{
	const float scale  = 1.5f;

    float a = 0.0f;
    float b = 0.5f;
	float f = 1.98f;
	glm::vec3 d = glm::vec3(0.0f);

	for (int i = 0; i < 8; i++) {
        a += b * noised(f * x * scale);           // accumulate values		
        b *= 0.49f;             // amplitude decrease
        f *= 1.8f;             // frequency increase
    }

	return a;
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */
glm::vec3 TerrainCliff::getPosition(int row, int col)
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

	float bicubicU = pow(u, 2) * (3 - 2 * u);
	float bicubicV = pow(v, 2) * (3 - 2 * v);

	float ab = glm::mix(aYPosition, bYPosition, bicubicU);
	float cd = glm::mix(cYPosition, dYPosition, bicubicU);

	position.y = glm::mix(ab, cd, bicubicV) / m_length_modifier;

	int newYRowCeiling = glm::ceil(row / 20.0f);
	int newYColCeiling = glm::ceil(col / 20.0f);

	// float distFromCenterSquared = position.x * position.x + position.z * position.z;

	// float noise = randValue(row, col) * 0.08 / (m_length_modifier * m_length_modifier);
	// float noise = noised(position);
	glm::vec3 newPos(position);
	// newPos.x = fabs(newPos.x);
	
	if (newPos.x < 0) {
		newPos.x = fabs(newPos.x - 0.03) * 2;
	}
	if (newPos.z < 0) {
		newPos.z = fabs(newPos.z - 0.1) * 2;
	}

	float noise = fabs(getNoise(newPos));
	// float noise = (fbmd(position).x);

	position.y = std::pow(noise * 1.1f, 3);
	position.y = pow(position.y, 2) + position.y * 2;

	float distFromCenterSquared = position.x * position.x + position.z * position.z;
	if (distFromCenterSquared > m_centerRadius * m_centerRadius) {
		position.y += randValue(row, col) * 0.008;
	}
	else {
		position.y += randValue(row, col) * 0.001;
	}

	// position.y = pow(position.y, 2) + position.y * 2 + noise;

	
	// position.y /= m_length_modifier;
	// position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08;
	// position.y = position.y * newYColCeiling + randValue(newYRowCeiling, newYColCeiling);
    // position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08 + 0.3 * randValue(newYRowCeiling, newYColCeiling);
	// position.y = randValue(newRow, newCol);

    return position;
}


TerrainCliff::~TerrainCliff()
{
}

