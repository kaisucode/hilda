#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"

#include <iostream>
#include <QImage>
#include <QString>

class Camera;
class CS123ISceneParser;


struct TransformedPrimitive {
	CS123ScenePrimitive primitive;
	glm::mat4x4 transformations;
	QImage textureImage;
};
// struct TransformedPrimitive {
//     TransformedPrimitive(CS123ScenePrimitive aPrimitive, glm::mat4x4 someTransformations)
//         : primitive(aPrimitive), transformations(someTransformations){}
// };

/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

	CS123SceneGlobalData m_global;
	std::vector<CS123SceneLightData> m_lights;
	std::vector<TransformedPrimitive> m_transformedPrimitives;

	glm::mat4x4 apply3dTransformations(glm::mat4x4 ogMatrix, CS123SceneTransformation* transMatrices);
	void recursivelyGetPrimitives(CS123SceneNode *node, std::vector<CS123SceneTransformation*> pastTransformations);

protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

};

#endif // SCENE_H
