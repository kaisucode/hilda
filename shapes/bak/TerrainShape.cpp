#include "shapes/TerrainShape.h"
#include "shapes/TreeShape.h"

TerrainShape::TerrainShape() :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
    m_isFilledIn(true)
{
    m_triangleLayout = CS123::GL::VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP;
//     m_triangleLayout= CS123::GL::VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES;
}

TerrainShape::TerrainShape(int param1, int param2) :
	m_numRows(m_length * m_length_modifier), 
	m_numCols(m_length * m_length_modifier), 
	m_isFilledIn(true)
{
    m_triangleLayout= CS123::GL::VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP;
//    m_triangleLayout= CS123::GL::VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES;
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
//    std::make_unique<TreeShape>(settings.shapeParameter1, settings.shapeParameter2);
//    std::unique_ptr<TreeShape> tree = std::make_unique<TreeShape>(m_vertexData);
//	int i = 1;
//    std::cout<<"hi"<<std::endl;
    initializeOpenGLShapeProperties();
}

int TerrainShape::getVertexDataSize() {
    return m_vertexData.size();
}

glm::vec3 TerrainShape::getVertexAtIndex(int index) {
   GLfloat x = m_vertexData.at(index);
   GLfloat y = m_vertexData.at(index+1);
   GLfloat z = m_vertexData.at(index+2);
   return {x,y,z};
}
/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and
 * column.
 */
float TerrainShape::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


/**
 * Returns the object-space position for the terrain vertex at the given row
 * and column.
 */
glm::vec3 TerrainShape::getPosition(int row, int col)
{
//    glm::vec3 position;
//    position.x = 10 * row/m_numRows - 5;
//    position.y = 0;
//    position.z = 10 * col/m_numCols - 5;

//    // TODO: Adjust position.y using value noise.
	
//    // position.y = randValue(row, col);

//    int newRowFloor = glm::floor(row / 20.0f);
//    int newColFloor = glm::floor(col / 20.0f);
//    int newRowCeiling = glm::ceil(row / 20.0f);
//    int newColCeiling = glm::ceil(col / 20.0f);
//    // position.y = randValue(newRow, newCol);

//    float aYPosition = randValue(newRowFloor, newColFloor);
//    float bYPosition = randValue(newRowFloor, newColCeiling);
//    float cYPosition = randValue(newRowCeiling, newColFloor);
//    float dYPosition = randValue(newRowCeiling, newColCeiling);

//    // float aYPosition = randValue(glm::floor(row / 20.0f), glm::floor(newCol);
//    // float bYPosition = randValue(newRow, newCol + 1);
//    // float cYPosition = randValue(newRow + 1, newCol);
//    // float dYPosition = randValue(newRow + 1, newCol + 1);


//    // u = (position.x - row * 20.0f) / 20.0f;
//    // v = (position.z - col * 20.0f) / 20.0f;

//    float u = glm::fract(col / 20.0f);
//    float v = glm::fract(row / 20.0f);

//    // float bicubicU = pow(u, 2) * (3 - 2 * u);
//    // float bicubicV = pow(v, 2) * (3 - 2 * v);

//    float bicubicU = pow(u, 2) * (3 - 2 * u);
//    float bicubicV = pow(v, 2) * (3 - 2 * v);

//    float ab = glm::mix(aYPosition, bYPosition, bicubicU);
//    float cd = glm::mix(cYPosition, dYPosition, bicubicU);

//    position.y = glm::mix(ab, cd, bicubicV) / m_length_modifier;

//    int newYRowCeiling = glm::ceil(row / 20.0f);
//    int newYColCeiling = glm::ceil(col / 20.0f);

//    float distFromCenterSquared = position.x * position.x + position.z * position.z;

//    float noise = 0.08 / (m_length_modifier * m_length_modifier);
//    // float noise = 0.08 / (m_length_modifier);

//    if (distFromCenterSquared < centerRadius * centerRadius) {
//        // position.y -= 5;
//        position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * noise / 2;
//        // position.y -= fabs(m_numRows - distFromCenterSquared) * 0.03 - 1;
//        // position.y -= fabs(m_numRows - distFromCenterSquared) * 0.01;
//    // position.x = 10 * row/m_numRows - 5;
//        position.y -= fabs(10 + 5 - distFromCenterSquared) * 0.25;
//    }
//    else if (distFromCenterSquared < centerRadius * centerRadius) {
//        // position.y -= 5;
//        position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * noise / 2;
//        // position.y -= fabs(m_numRows - distFromCenterSquared) * 0.03 - 1;
//        // position.y -= fabs(m_numRows - distFromCenterSquared) * 0.01;
//    // position.x = 10 * row/m_numRows - 5;
//        position.y -= fabs(10 + 5 - distFromCenterSquared) * 0.25;
//    }
//    else {
//        position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * noise;
//    }
	
//    // position.y /= m_length_modifier;
//    // position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08;
//    // position.y = position.y * newYColCeiling + randValue(newYRowCeiling, newYColCeiling);
//    // position.y = pow(position.y, 2) + position.y * 2 + randValue(row, col) * 0.08 + 0.3 * randValue(newYRowCeiling, newYColCeiling);
//    // position.y = randValue(newRow, newCol);

//    return position;

    glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.

    float frequency = 20.0;
    float amplitude = 1.0;

    for (int i = 0; i < 3; i++) {
        int row_5 = glm::floor(row / frequency);
        int col_5 = glm::floor(col / frequency);

        float A = randValue(row_5, col_5);
        float B = randValue(row_5, col_5 + 1);
        float C = randValue(row_5 + 1, col_5);
        float D = randValue(row_5 + 1, col_5 + 1);

        float x = glm::fract(col / frequency);

        float A_B_mix = glm::mix(A, B, 3 * x * x - 2 * x * x * x);
        float C_D_mix = glm::mix(C, D, 3 * x * x - 2 * x * x * x);

        float y = glm::fract(row / frequency);

        position.y += glm::mix(A_B_mix, C_D_mix, 3 * y * y - 2 * y * y * y) / amplitude;

        frequency /= 2.0;
        amplitude *= 2.0;
    }

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
