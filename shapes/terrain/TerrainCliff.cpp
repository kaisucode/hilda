#include "shapes/TerrainShape.h"

TerrainShape::TerrainShape() :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
}

TerrainShape::TerrainShape(int param1, int param2) :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
	m_param1 = std::max(1, param1); // number of layers of triangles
	m_param2 = std::max(3, param2); // number of sides

	int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);
	m_vertexData.reserve(2 * numVertices * 3);

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
float TerrainShape::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


float hash(glm::vec3 p)  // replace this by something better
{
    p  = 50.0f * glm::fract(p * 0.3183099f + glm::vec3(0.71,0.113,0.419));
    return -1.0 + 2.0 * glm::fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}


glm::vec4 noised(glm::vec3 x)
{
	glm::vec3 i = glm::floor(x / 20.0f);
	glm::vec3 w = glm::fract(x / 20.0f);
    
    // quintic interpolation
	glm::vec3 u = w*w*w*(w*(w*6.0f - 15.0f)+10.0f);
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

	glm::vec3 uyzx = glm::vec3(u.y, u.z, u.x);
	glm::vec3 uzxy = glm::vec3(u.z, u.x, u.y);

	// return k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z;
	return glm::vec4( k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z, \
                 du * (glm::vec3(k1,k2,k3) + uyzx*glm::vec3(k4,k5,k6) + uzxy*glm::vec3(k6,k4,k5) + k7*uyzx*uzxy ));

}

glm::vec4 fbmd( glm::vec3 x )
{
	const float scale  = 1.5f;
    // const float scale  = 3.0f;

    float a = 0.0f;
    float b = 0.5f;
	// float f = 1.0f;
	float f = 1.98f;
	glm::vec3 d = glm::vec3(0.0f);

	for (int i = 0; i < 16; i++) {
		glm::vec4 noisedOutput = noised(f*x*scale);
        a += b * noisedOutput.x;           // accumulate values		
        d += b * glm::vec3(noisedOutput.y, noisedOutput.z, noisedOutput.w) * f * scale; // accumulate derivatives
        b *= 0.49f;             // amplitude decrease
        f *= 1.8f;             // frequency increase
    }

	return glm::vec4(a, d);
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */
glm::vec3 TerrainShape::getPosition(int row, int col)
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
	float noise = fabs(fbmd(position).x);
	// float noise = (fbmd(position).x);

	position.y = std::pow(noise * 1.1f, 3);
	position.y = pow(position.y, 2) + position.y * 2;

	// position.y = pow(position.y, 2) + position.y * 2 + noise;

	
	// position.y /= m_length_modifier;
	// position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08;
	// position.y = position.y * newYColCeiling + randValue(newYRowCeiling, newYColCeiling);
    // position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08 + 0.3 * randValue(newYRowCeiling, newYColCeiling);
	// position.y = randValue(newRow, newCol);

    return position;
}


/**
 * Returns the normal vector for the terrain vertex at the given row and
 * column.
 */
glm::vec3 TerrainShape::getNormal(int row, int col)
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

bool TerrainShape::isFilledIn() {
    return m_isFilledIn;
}


TerrainShape::~TerrainShape()
{
}

