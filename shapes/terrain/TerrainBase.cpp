#include "shapes/terrain/TerrainBase.h"

TerrainBase::TerrainBase() :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
}

TerrainBase::TerrainBase(int param1, int param2) :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
	m_param1 = std::max(1, param1); // number of layers of triangles
	m_param2 = std::max(3, param2); // number of sides

	int numVertices = (m_numRows - 1) * (2 * m_numCols + 2);
	m_vertexData.reserve(2 * numVertices * 3);
}



/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and
 * column.
 */
float TerrainBase::randValue(int row, int col)
{
	return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


// float hash(glm::vec3 p)  // replace this by something better
// {
//     p  = 50.0f * glm::fract(p * 0.3183099f + glm::vec3(0.71,0.113,0.419));
//     return -1.0 + 2.0 * glm::fract(p.x * p.y * p.z * (p.x + p.y + p.z));
// }


// glm::vec4 noised(glm::vec3 x)
// {
//     glm::vec3 i = glm::floor(x / 20.0f);
//     glm::vec3 w = glm::fract(x / 20.0f);
    
//     // quintic interpolation
//     glm::vec3 u = w*w*w*(w*(w*6.0f - 15.0f)+10.0f);
//     glm::vec3 du = 30.0f*w*w*(w*(w-2.0f)+1.0f);
    
//     float temp = 20.0f;
//     float a = hash(i + glm::vec3(0.0,0.0,0.0));
//     float b = hash(i + glm::vec3(temp,0.0,0.0));
//     float c = hash(i + glm::vec3(0.0,temp,0.0));
//     float d = hash(i + glm::vec3(temp,temp,0.0));
//     float e = hash(i + glm::vec3(0.0,0.0,temp));
//     float f = hash(i + glm::vec3(temp,0.0,temp));
//     float g = hash(i + glm::vec3(0.0,temp,temp));
//     float h = hash(i + glm::vec3(temp, temp, temp));
	
//     float k0 =   a;
//     float k1 =   b - a;
//     float k2 =   c - a;
//     float k3 =   e - a;
//     float k4 =   a - b - c + d;
//     float k5 =   a - c - e + g;
//     float k6 =   a - b - e + f;
//     float k7 = - a + b + c - d + e - f - g + h;

//     glm::vec3 uyzx = glm::vec3(u.y, u.z, u.x);
//     glm::vec3 uzxy = glm::vec3(u.z, u.x, u.y);

//     // return k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z;
//     return glm::vec4( k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z, \
//                  du * (glm::vec3(k1,k2,k3) + uyzx*glm::vec3(k4,k5,k6) + uzxy*glm::vec3(k6,k4,k5) + k7*uyzx*uzxy ));

// }

float TerrainBase::getNoise( glm::vec3 x )
{
	return 0.0f;
	// const float scale  = 1.5f;
	// // const float scale  = 3.0f;

	// float a = 0.0f;
	// float b = 0.5f;
	// // float f = 1.0f;
	// float f = 1.98f;
	// glm::vec3 d = glm::vec3(0.0f);

	// for (int i = 0; i < 16; i++) {
	//     glm::vec4 noisedOutput = noised(f*x*scale);
	//     a += b * noisedOutput.x;           // accumulate values		
	//     d += b * glm::vec3(noisedOutput.y, noisedOutput.z, noisedOutput.w) * f * scale; // accumulate derivatives
	//     b *= 0.49f;             // amplitude decrease
	//     f *= 1.8f;             // frequency increase
	// }

	// // return glm::vec4(a, d);
	// return a;
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */

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

