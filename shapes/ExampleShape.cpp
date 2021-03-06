#include "ExampleShape.h"
#include <iostream>

ExampleShape::ExampleShape()
{
}

ExampleShape::ExampleShape(int param1, int param2) :
    m_param1(param1),
    m_param2(param2)
{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */
    m_vertexData = CUBE_DATA_POSITIONS;

	// for (int i = 0; i < m_vertexData.size(); i += 6) {
	//     printf("--------------\n");
	//     printf("%f ", m_vertexData.at(i + 3));
	//     printf("%f ", m_vertexData.at(i + 4));
	//     printf("%f\n", m_vertexData.at(i + 5));
	// }

    /**
     * Initialize the OpenGLShapeProperties so that the shape is ready to be drawn.
     * Note that this does not actually draw the shape, that happens in ShapesScene.cpp
     */
    initializeOpenGLShapeProperties();
}

ExampleShape::~ExampleShape()
{
}
