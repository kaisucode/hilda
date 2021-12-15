#ifndef BASICTREE_H
#define BASICTREE_H

#include "shapes/CylinderShape.h"
#include "shapes/ConeShape.h"
#include "gl/shaders/CS123Shader.h"

class BasicTree
{
public:
    BasicTree(); 
    void draw(std::unique_ptr<CS123::GL::CS123Shader> &shader, glm::mat4 &translation);

private:
    std::unique_ptr<CylinderShape> m_cylinder;
    std::unique_ptr<ConeShape> m_cone;
};

#endif // BASICTREE_H
