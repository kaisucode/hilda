#ifndef TERRAINSHAPE_H
#define TERRAINSHAPE_H

#include "OpenGLShape.h"

class TerrainShape : public OpenGLShape
{
public:
    TerrainShape();
    TerrainShape(int param1, int param2);
    ~TerrainShape();
	bool isFilledIn();

private: 
	float randValue(int row, int col);
	glm::vec3 getPosition(int row, int col);
	glm::vec3 getNormal(int row, int col);

	const float m_length = 100;
	const float m_length_modifier = 3;
	const float centerRadius = 4;
	const float flatSection = 1;

	const float m_numRows;
	const float m_numCols;
	const bool m_isFilledIn;

};

#endif // TERRAINSHAPE_H
