#include "BasicTree.h"
#include "Settings.h"

BasicTree::BasicTree() :
    m_cylinder(std::make_unique<CylinderShape>(2, 7)),
    m_cone(std::make_unique<ConeShape>(2, 7)),
    m_scaleFactor(0.25f),
    m_leafColor(0.090, 0.368, 0.098),
    m_trunkColor(0.505, 0.345, 0.254)
{

}

void BasicTree::draw(std::unique_ptr<CS123::GL::CS123Shader> &shader, glm::vec3 translation) {
    shader->setUniform("terrain", false);
    glm::mat4 fullTreeModel = glm::translate(translation) * glm::scale(glm::vec3(m_scaleFactor));

    shader->setUniform("objectColor", m_trunkColor);

    float treeOutline = 0.5f * m_scaleFactor * settings.outlineWeight;
    shader->setUniform("outlineWeight", treeOutline);

    glm::mat4 primitiveModel = glm::scale(glm::vec3(0.25, 0.5, 0.25));
    shader->setUniform("m", fullTreeModel * primitiveModel);
    m_cylinder->draw();

    shader->setUniform("objectColor", m_leafColor);
    treeOutline = 1.5f * m_scaleFactor * settings.outlineWeight;
    shader->setUniform("outlineWeight", treeOutline);

    primitiveModel = glm::translate(glm::vec3(0, 0.75, 0));
    shader->setUniform("m", fullTreeModel * primitiveModel);
    m_cone->draw();

    primitiveModel = glm::translate(glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.75));
    shader->setUniform("m", fullTreeModel * primitiveModel);
    m_cone->draw();

    primitiveModel = glm::translate(glm::vec3(0, 1.25, 0)) * glm::scale(glm::vec3(0.6));
    shader->setUniform("m", fullTreeModel * primitiveModel);
    m_cone->draw();
}
