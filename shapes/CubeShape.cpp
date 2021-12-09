#include "CubeShape.h"
#include <iostream>

CubeShape::CubeShape()
{
}

CubeShape::CubeShape(int param1, int param2)
{
	m_param1 = std::max(1, param1); 
	m_param2 = std::max(1, param2);

	// go through each face
	for (int faceIndex = 0; faceIndex < cubeFaces.size(); faceIndex++) {

		int startingCornerIndex = 0;
		std::vector<Vertex> curFace = cubeFaces.at(faceIndex);

		// vertex indexing
		std::vector<std::vector<Vertex>> vertexInfo;
		vertexInfo.reserve((m_param1 + 1) * (m_param2 + 1));

		for (int i = 0; i <= m_param1; i++) {
			std::vector<Vertex> aRow;
			for (int j = 0; j <= m_param1; j++) {

				float a, b, c;
				// change indexes to match different faces

				if (faceIndex == 2) {
					a = curFace.at(startingCornerIndex).pos.x + i * (1.0f / m_param1);
					b = curFace.at(startingCornerIndex).pos.y + j * (1.0f / m_param1) - 1;
					c = curFace.at(startingCornerIndex).pos.z + 2;
				}
				else if (faceIndex == 3) {
					a = curFace.at(startingCornerIndex).pos.x + i * (1.0f / m_param1);
					b = curFace.at(startingCornerIndex).pos.y + j * (1.0f / m_param1);
					c = curFace.at(startingCornerIndex).pos.z + 1;
				}
				else {
					a = curFace.at(startingCornerIndex).pos.x + i * (1.0f / m_param1);
					b = curFace.at(startingCornerIndex).pos.y + j * (1.0f / m_param1);
					c = curFace.at(startingCornerIndex).pos.z;
				}

				std::vector<float> coor = { a, b, c };

				std::vector<int> orderIdxes = indexOrdering[faceIndex];

				Vertex aVertex = Vertex(coor.at(orderIdxes.at(0)), \
						coor.at(orderIdxes.at(1)) - 1.0f, \
						coor.at(orderIdxes.at(2)),\
						cubeFaceNormals.at(faceIndex)); 

				// Vertex aVertex = Vertex(a, b, c); // for front face
				// Vertex aVertex = Vertex(b, a, c); // for back face
				aRow.push_back(aVertex);
			}
			vertexInfo.push_back(aRow);
		}

		this->addGridToVertex(vertexInfo, m_param1, m_param1);
	}


	/**
	 * Initialize the OpenGLShapeProperties so that the shape is ready to be drawn.
	 * Note that this does not actually draw the shape, that happens in ShapesScene.cpp
	 */
	initializeOpenGLShapeProperties();
}

CubeShape::~CubeShape()
{
}
