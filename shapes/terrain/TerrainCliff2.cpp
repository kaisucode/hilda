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

float TerrainCliff::getNoise( glm::vec3 x )
{
	const float scale  = 1.5f;
    // const float scale  = 3.0f;

    float a = 0.0f;
    float b = 0.5f;
	float f = 1.98f;
	glm::vec3 d = glm::vec3(0.0f);

	for (int i = 0; i < 16; i++) {
		glm::vec4 noisedOutput = noised(f*x*scale);
        a += b * noisedOutput.x;           // accumulate values		
        d += b * glm::vec3(noisedOutput.y, noisedOutput.z, noisedOutput.w) * f * scale; // accumulate derivatives
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

	float noise = fabs(getNoise(position));
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


TerrainCliff::~TerrainCliff()
{
}

