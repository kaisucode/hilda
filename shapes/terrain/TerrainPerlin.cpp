#include "shapes/terrain/TerrainPerlin.h"

TerrainPerlin::TerrainPerlin()
{
	TerrainBase();
}

TerrainPerlin::TerrainPerlin(int param1, int param2)
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
float TerrainPerlin::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


float TerrainPerlin::hash(glm::vec3 p)
{
    p  = 50.0f * glm::fract(p * 0.3183099f + glm::vec3(0.71,0.113,0.419));
	return -1.0 + 2.0 * glm::fract(p.x * p.y * p.z * (p.x + p.y + p.z));
}

// glm::vec3 TerrainPerlin::hash3D(glm::vec3 p)
glm::vec3 hash3D(glm::vec3 p)
{
	p = glm::vec3( glm::dot(p,glm::vec3(127.1,311.7, 74.7)),
			glm::dot(p,glm::vec3(269.5,183.3,246.1)),
			glm::dot(p,glm::vec3(113.5,271.9,124.6)));

	return -1.0f + 2.0f*glm::fract(glm::sin(p)*43758.5453123f);
}


float TerrainPerlin::noised(glm::vec3 x)
{
	float temp = 1.1f;

	glm::vec3 i = glm::floor(x / temp);
	glm::vec3 w = glm::fract(x / temp);
    
    // quintic interpolation
	glm::vec3 u = w * w * w * (w * (w * 6.0f - 15.0f) + 10.0f);
	// glm::vec3 u = w * w * (3.0f - 2.0f * w);

	glm::vec3 ga = hash3D( i+glm::vec3(0.0,0.0,0.0) );
	glm::vec3 gb = hash3D( i+glm::vec3(temp,0.0,0.0) );
	glm::vec3 gc = hash3D( i+glm::vec3(0.0,temp,0.0) );
	glm::vec3 gd = hash3D( i+glm::vec3(temp,temp,0.0) );
	glm::vec3 ge = hash3D( i+glm::vec3(0.0,0.0,temp) );
	glm::vec3 gf = hash3D( i+glm::vec3(temp,0.0,temp) );
	glm::vec3 gg = hash3D( i+glm::vec3(0.0,temp,temp) );
	glm::vec3 gh = hash3D( i+glm::vec3(temp,temp,temp) );
    
	float va = glm::dot( ga, w-glm::vec3(0.0,0.0,0.0) );
	float vb = glm::dot( gb, w-glm::vec3(temp,0.0,0.0) );
	float vc = glm::dot( gc, w-glm::vec3(0.0,temp,0.0) );
	float vd = glm::dot( gd, w-glm::vec3(temp,temp,0.0) );
	float ve = glm::dot( ge, w-glm::vec3(0.0,0.0,temp) );
	float vf = glm::dot( gf, w-glm::vec3(temp,0.0,temp) );
	float vg = glm::dot( gg, w-glm::vec3(0.0,temp,temp) );
	float vh = glm::dot( gh, w-glm::vec3(temp,temp,temp) );

	return va + 
		u.x*(vb-va) + 
		u.y*(vc-va) + 
		u.z*(ve-va) + 
		u.x*u.y*(va-vb-vc+vd) + 
		u.y*u.z*(va-vc-ve+vg) + 
		u.z*u.x*(va-vb-ve+vf) + 
		u.x*u.y*u.z*(-va+vb+vc-vd+ve-vf-vg+vh);

    // float a = hash(i + glm::vec3(0.0,0.0,0.0));
    // float b = hash(i + glm::vec3(temp,0.0,0.0));
    // float c = hash(i + glm::vec3(0.0,temp,0.0));
    // float d = hash(i + glm::vec3(temp,temp,0.0));
    // float e = hash(i + glm::vec3(0.0,0.0,temp));
	// float f = hash(i + glm::vec3(temp,0.0,temp));
    // float g = hash(i + glm::vec3(0.0,temp,temp));
    // float h = hash(i + glm::vec3(temp, temp, temp));
	
    // float k0 =   a;
    // float k1 =   b - a;
    // float k2 =   c - a;
    // float k3 =   e - a;
    // float k4 =   a - b - c + d;
    // float k5 =   a - c - e + g;
    // float k6 =   a - b - e + f;
    // float k7 = - a + b + c - d + e - f - g + h;

	// return k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z;

}

float TerrainPerlin::getNoise( glm::vec3 x )
{

	return noised(x);
	const float scale  = 1.5f;

    float a = 0.0f;
    float b = 0.5f;
	float f = 1.98f;
	glm::vec3 d = glm::vec3(0.0f);

	for (int i = 0; i < 1; i++) {
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
glm::vec3 TerrainPerlin::getPosition(int row, int col)
{
    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
	position.y = randValue(row, col);
	glm::vec3 newPos(position);
	// std::cout << "got position: " << std::endl;

	// float noise = fabs(getNoise(newPos));
	float noise = (getNoise(newPos));
	// std::cout << "got noise: " << noise << std::endl;
	// float noise = (fbmd(position).x);

	position.y = std::pow(noise * 1.1f, 3);
	// position.y = pow(noise, 2) + position.y * 2;
	position.y += 0.2f;
	// position.y += randValue(row, col) * 0.001;

    return position;
}


TerrainPerlin::~TerrainPerlin()
{
}

