#ifndef BASICTREE_H
#define BASICTREE_H

#include "shapes/CylinderShape.h"
#include "shapes/ConeShape.h"
#include "gl/shaders/CS123Shader.h"

class BasicTree
{
public:
    BasicTree(); 
    void draw(std::unique_ptr<CS123::GL::CS123Shader> &shader, glm::vec3 translation);

private:
    std::unique_ptr<CylinderShape> m_cylinder;
    std::unique_ptr<ConeShape> m_cone;

    float m_scaleFactor;

    glm::vec3 m_leafColor;
    glm::vec3 m_trunkColor;
};

#endif // BASICTREE_H
