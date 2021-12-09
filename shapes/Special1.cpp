#include "shapes/SphereShape.h"

SphereShape::SphereShape()
{
}

SphereShape::SphereShape(int param1, int param2) 
{
	// m_param1 = param1; // number of layers of triangles
	// m_param2 = param2; // number of sides
	m_param1 = std::max(3, param1);
	m_param2 = std::max(3, param2);
	
	// build circumference vertices (initialized with y-values of 0s)
	std::vector<Vertex> circumferenceVertices = this->buildCircumferenceVertices(m_param2);

	// this->buildCap(circumferenceVertices, 0.5, true);
	// this->buildCap(circumferenceVertices, -0.5, false);
	this->buildSides(circumferenceVertices);

	initializeOpenGLShapeProperties();
}

void SphereShape::buildSides(std::vector<Vertex> circumferenceVertices) {
	std::vector<std::vector<Vertex>> vertexInfo;

	std::vector<Vertex> verticalCircumferenceVertices = this->buildCircumferenceVertices(m_param1);

	for (int numOfSides = 0; numOfSides < m_param2; numOfSides++) 
	{
		vertexInfo.clear();

		// Vertex bottomLeftVertex = circumferenceVertices.at(numOfSides);
		// Vertex bottomRightVertex = circumferenceVertices.at(numOfSides + 1);

		GLfloat r = 0.5f;
		GLfloat theta = 2 * M_PI * (numOfSides % m_param2) / m_param2;
		GLfloat nextTheta = 2 * M_PI * ((numOfSides + 1) % m_param2) / m_param2;

		std::cout << "34" << std::endl;
		for (int row = 0; row <= m_param1; row++) {

			GLfloat alpha = 2 * M_PI * (row % m_param1) / m_param1;
			GLfloat nextAlpha = 2 * M_PI * (row % m_param1) / m_param1;

			Vertex leftVertex = Vertex(r * glm::cos(theta) * glm::cos(alpha),\
						r *	glm::sin(theta),\
						r * glm::cos(theta) * glm::sin(alpha));

			Vertex rightVertex = Vertex(r * glm::cos(nextTheta) * glm::cos(alpha),\
						r *	glm::sin(nextTheta),\
						r * glm::cos(nextTheta) * glm::sin(alpha));
			

			// Vertex leftVertexReference = verticalCircumferenceVertices.at((row + 1) % m_param1);
			// Vertex rightVertexReference = verticalCircumferenceVertices.at((row + 1) % m_param1);

			// Vertex leftVertex = Vertex(leftVertexReference.pos.y, leftVertexReference.pos.z, leftVertexReference.pos.x);
			// Vertex rightVertex = Vertex(rightVertexReference.pos.y, rightVertexReference.pos.z, rightVertexReference.pos.x);


			// glm::mat3 rotationMatrix = { { 1, 0, 0 }, { 0, 0, -1 }, { 0, 1, 0 }};


			// leftVertex = glRotatef(90.0f, 1, 0, 0);
			// leftVertex = rotationMatrix * leftVertexReference;
			// rightVertex = rightVertexReference * rotationMatrix;

			// glm::mat4 rotationMatrix;

			// rotationMatrix = glm::mat4(1.0f);
			// rotationMatrix = glm::rotate(rotationMatrix, 90.f, glm::vec3(1, 0, 0));
			// leftVertex.pos = glm::vec3(rotationMatrix * glm::vec4(leftVertexReference.pos, 1.0));
			// rightVertex.pos = glm::vec3(rotationMatrix * glm::vec4(rightVertexReference.pos, 1.0));

			// rotationMatrix = glm::mat4(1.0f);
			// rotationMatrix = glm::rotate(rotationMatrix, 90.f, glm::vec3(1, 0, 0));
			// leftVertex.pos = glm::vec3(rotationMatrix * glm::vec4(leftVertex.pos, 1.0));
			// rightVertex.pos = glm::vec3(rotationMatrix * glm::vec4(rightVertex.pos, 1.0));

			// leftVertex = glm::rotateX(leftVertexReference, 90.0f);
			// rightVertex = glm::rotateX(rightVertexReference, 90.0f);

			// glm::mat4 rotationMatrix = glm::rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
			// glm::mat4 rotationMatrix = glm::rotate(3.14f, glm::vec3(1.0f, 0.0f, 0.f));
			//

			// glm::mat4 leftVertexEX = leftVertexReference.pos * rotationMatrix;
			// glm::mat4 leftVertexEX = glm::scale(rotationMatrix, leftVertexReference.pos);
			// std::cout << glm::to_string() << std::endl;

			// leftVertex.pos = rotationMatrix * leftVertexReference.pos;
			// rightVertex.pos = rotationMatrix * rightVertexReference.pos;
			// leftVertex = glm::rotate(leftVertexReference.pos, 90.0f, glm::vec3(1, 0, 0));
			// rightVertex = glm::rotate(rightVertexReference.pos, 90.f, glm::vec3(1, 0, 0));
 
			vertexInfo.push_back({ leftVertex, rightVertex });
		}
		std::cout << "44" << std::endl;

		// if (m_param1 % 2 == 0) { // even num of rows
		//     bottomLeftVertex.pos.y = 0;
		//     bottomRightVertex.pos.y = 0;
		// }
		// else {
		// }

		// build up and down

		// for (int layer = 0; layer < (param2 / 2); layer++) {

		// }

		// bottomLeftVertex.pos.y = -0.5f;
		// bottomRightVertex.pos.y = -0.5f;

		// for (int i = 0; i <= m_param2; i++) {
		//     Vertex leftVertex = bottomLeftVertex;
		//     Vertex rightVertex = bottomRightVertex;

		//     leftVertex.pos.y += i * (1.0f) / m_param2;
		//     rightVertex.pos.y += i * (1.0f) / m_param2;

		//     vertexInfo.push_back({ leftVertex, rightVertex });
		// }

		for (int row = 0; row < m_param1; row++) {
			Vertex p1, p2, p3;
			std::cout << "76" << std::endl;
			// std::cout << "vertexInfo sizes: " << vertexInfo.size() << std::endl;
			// for (int a = 0; a < vertexInfo.size(); a++) {
			//     std::cout << "vertexInfo.at(" << a << ") sizes: " << vertexInfo.at(a).size() << std::endl;
			// }

			p1 = vertexInfo.at(row).at(0);
			p2 = vertexInfo.at(row + 1).at(0);
			p3 = vertexInfo.at(row + 1).at(1);
			TriangleShape firstTriangle = TriangleShape(p1, p2, p3);
			addTriToVertexData(firstTriangle);

			p1 = vertexInfo.at(row + 1).at(1);
			p2 = vertexInfo.at(row).at(1);
			p3 = vertexInfo.at(row).at(0);
			TriangleShape secondTriangle = TriangleShape(p1, p2, p3);
			addTriToVertexData(secondTriangle);

		}
	}
}


SphereShape::~SphereShape()
{
}

