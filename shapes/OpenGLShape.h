#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/rotate_vector.hpp>

#include "GL/glew.h"

#include "Vertex.h"
#include "TriangleShape.h"

#include <memory>
#include <vector>
#include <math.h>
#include <iostream>

#include "gl/datatype/VBO.h"

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

namespace CS123 { namespace GL {
class VAO;
}}

class OpenGLShape
{
public:
    OpenGLShape();
    virtual ~OpenGLShape();
    void draw();

	std::vector<Vertex>	buildCircumferenceVertices(int numOfVertices);
	void buildCap(std::vector<Vertex> circumferenceVertices, GLfloat yValue, bool orientationIsUp);
	void buildCapSlice(Vertex bottomLeftVertex, Vertex bottomRightVertex,\
			Vertex centerVertex, Vertex curLeftVertex, Vertex curRightVertex,\
			bool orientationIsUp = true);

	void addTriToVertexData(TriangleShape aTriangle, bool shouldReverse = false);
	void addGridToVertex(std::vector<std::vector<Vertex>> vertexInfo,\
			int maxRows, int maxCols, bool shouldReverse = false);
	void setShapeType(int newShapeType);

	int shapeType;
    int m_param1;
    int m_param2;

protected:
    /**
     * initializes the relavant openGL properties for the shape
     * don't worry about what exactly this function is doing, you'll learn more about that later in the course!
     * look at ExampleShape.cpp for it's demonstrated usage
     */
    void initializeOpenGLShapeProperties();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
    GLfloat m_radius = 0.5f; 

    CS123::GL::VBO::GEOMETRY_LAYOUT m_triangleLayout;
};

#endif // OPENGLSHAPE_H
