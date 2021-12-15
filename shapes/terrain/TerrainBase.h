#ifndef TERRAINBASE_H
#define TERRAINBASE_H

#include "shapes/OpenGLShape.h"

class TerrainBase : public OpenGLShape
{
public:
    TerrainBase();
    TerrainBase(int param1, int param2);
    ~TerrainBase();
	bool isFilledIn();

	const float m_length = 100;
	const float m_length_modifier = 3;
	const float centerRadius = 4;
	const float flatSection = 1;

	const float m_numRows;
	const float m_numCols;
	const bool m_isFilledIn;

	virtual float randValue(int row, int col);
	virtual float getNoise(glm::vec3 pos);
	virtual glm::vec3 getPosition(int row, int col) { return glm::vec3(0); };

	glm::vec3 getNormal(int row, int col);

    int getVertexDataSize();
    glm::vec3 getVertexAtIndex(int index);
};

#endif // TERRAINBASE_H
