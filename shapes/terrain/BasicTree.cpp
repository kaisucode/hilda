#include "BasicTree.h"
#include "Settings.h"

BasicTree::BasicTree() :
    m_cylinder(std::make_unique<CylinderShape>(2, 7)),
    m_cone(std::make_unique<ConeShape>(2, 7))
{

}

void BasicTree::draw(std::unique_ptr<CS123::GL::CS123Shader> &shader, glm::mat4 &translation) {
    shader->setUniform("terrain", false);
    shader->setUniform("objectColor", glm::vec3(0.090, 0.368, 0.098));
    float treeOutline = 0.25f * settings.outlineWeight;
    shader->setUniform("outlineWeight", treeOutline);
    glm::mat4 scaleMatrix = glm::scale(glm::vec3(0.25f));

    glm::mat4 modelMatrix = translation * scaleMatrix * glm::translate(glm::vec3(0, 0.25, 0));
    shader->setUniform("m", modelMatrix);
    m_cone->draw();

    modelMatrix *= glm::scale(glm::vec3(0.75)) * glm::translate(glm::vec3(0, 0.5, 0));
    shader->setUniform("m", modelMatrix);
    m_cone->draw();

    modelMatrix *= glm::scale(glm::vec3(0.75)) * glm::translate(glm::vec3(0, 0.5, 0));
    shader->setUniform("m", modelMatrix);
    m_cone->draw();

    treeOutline = 0.1f * settings.outlineWeight;
    shader->setUniform("outlineWeight", treeOutline);

    modelMatrix = translation * glm::scale(glm::vec3(0.25, 0.5, 0.25)) * scaleMatrix * glm::translate(glm::vec3(0, -0.6, 0));
    shader->setUniform("m", modelMatrix);
    m_cylinder->draw();
}
