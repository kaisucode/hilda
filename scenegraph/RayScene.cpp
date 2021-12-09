#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>
#include <cmath>


RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.
}

RayScene::~RayScene()
{
}

bool RayScene::getClosestIntersection(IntersectPoint *intersectPt, glm::vec4 d, glm::vec4 pEye) {

	// store vector of rays (transformed p's and t's, basically objspaceP and t)
	std::vector<IntersectPoint> minRayCandidates;

	for (int i = 0; i < m_transformedPrimitives.size(); i++) {

		// convert to object space
		TransformedPrimitive aTransformedPrimitive = m_transformedPrimitives.at(i);
		glm::mat4 inverseTransformation = glm::inverse(aTransformedPrimitive.transformations);

		glm::vec4 objSpacePEye = inverseTransformation * pEye;
		glm::vec4 objSpaceD = inverseTransformation * d;
		std::pair<GLfloat, glm::vec4> tValueAndNormal;

		std::unique_ptr<RayBaseShape> aRayShape;
		switch (aTransformedPrimitive.primitive.type) {
			case PrimitiveType::PRIMITIVE_CUBE:
				aRayShape = std::make_unique<RayCube>(objSpacePEye, objSpaceD, aTransformedPrimitive);
				break;
			case PrimitiveType::PRIMITIVE_CONE:
				aRayShape = std::make_unique<RayCone>(objSpacePEye, objSpaceD, aTransformedPrimitive);
				break;
			case PrimitiveType::PRIMITIVE_CYLINDER:
				aRayShape = std::make_unique<RayCylinder>(objSpacePEye, objSpaceD, aTransformedPrimitive);
				break;
			case PrimitiveType::PRIMITIVE_SPHERE:
				aRayShape = std::make_unique<RaySphere>(objSpacePEye, objSpaceD, aTransformedPrimitive);
				break;
			default:
				aRayShape = std::make_unique<RayBaseShape>(objSpacePEye, objSpaceD, aTransformedPrimitive);
		}


		// only consider valid t's, since t-values here would always be positive
		if (aRayShape->findIntersection()) { 
			IntersectPoint anIntersectPt = aRayShape->minIntersectPt;
			minRayCandidates.push_back(anIntersectPt);
		}
	}

	// find the min ray candidate from the vector
	if (minRayCandidates.size() != 0) {
		*intersectPt = minRayCandidates.at(0);
		for (int i = 1; i < minRayCandidates.size(); i++) {
			if (minRayCandidates.at(i).t < intersectPt->t) {
				*intersectPt = minRayCandidates.at(i);
			}
		}
		return intersectPt->isValid;
	}
	else {
		return false;
	}
}


glm::vec4 RayScene::getColor(glm::vec4 startingPoint, IntersectPoint intersectPt, int depth) {

	glm::vec4 n = intersectPt.getNormalWorldSpace();
	glm::vec4 p = intersectPt.getPointOfIntersectionWorldSpace();
	CS123SceneMaterial aMaterial = intersectPt.transformedPrimitive.primitive.material;

	// return (n / 2.0f + 0.5f);

	// illumination per channel
	glm::vec4 finalIntensity = m_global.ka * aMaterial.cAmbient;

	glm::vec4 cTexture(0);
	if (aMaterial.textureMap.isUsed) {
		cTexture = intersectPt.getTextureColor();
	}

	for (int lightIndex = 0; lightIndex < this->m_lights.size(); lightIndex++) {

		CS123SceneLightData aLight = m_lights.at(lightIndex);
		glm::vec4 intersectionToLight;
		GLfloat attenuation;

		switch (aLight.type) {
			case LightType::LIGHT_POINT: {
				intersectionToLight = aLight.pos - p;
				GLfloat d = glm::length(intersectionToLight);
				attenuation = std::min(1 / 
						(aLight.function.x + aLight.function.y * d + aLight.function.z * d * d), 1.0f);
				break;
			}
			case LightType::LIGHT_DIRECTIONAL: {
				intersectionToLight = -aLight.dir;
				attenuation = 1;
				break;
		  	}
			default:
		  		continue;
		}

		glm::vec4 cameraSpaceToEyeN = glm::normalize(startingPoint - p);
		glm::vec4 Lm = glm::normalize(intersectionToLight);
		glm::vec4 R = glm::normalize(2.0f * n * glm::dot(n, intersectionToLight) - intersectionToLight);

		if (settings.useShadows) {
			// to prevent self intersection
			glm::vec4 surfacePoint = intersectPt.getPointOfIntersectionWorldSpace() + Lm * m_epsilon;
			IntersectPoint newIntersectPt;
			bool objectInWay = getClosestIntersection(&newIntersectPt, Lm, surfacePoint);
			if (objectInWay) { // shadow, do not contribute this particular light to the summation
				continue;
			}
		}

		for (int i = 0; i < 3; i++) {
			GLfloat diffuseComponent;
			if (settings.useTextureMapping && aMaterial.textureMap.isUsed) {
				GLfloat blend = aMaterial.blend;
				diffuseComponent = (blend * cTexture[i] / 255.0f \
						+ (1 - blend) * m_global.kd * aMaterial.cDiffuse[i]) \
								   * fmax(0.0f, glm::dot(n, Lm));
			}
			else {
				diffuseComponent = m_global.kd * aMaterial.cDiffuse[i] * fmax(0.0f, glm::dot(n, Lm));
			}

			GLfloat specularComponent = m_global.ks * aMaterial.cSpecular[i] * \
										pow(fmax(0.0f, glm::dot(cameraSpaceToEyeN, R)), aMaterial.shininess);

			GLfloat newValue = aLight.color[i] * attenuation * (diffuseComponent + specularComponent);
			finalIntensity[i] += newValue;
		}
	}

	if (settings.useReflection && depth < m_maxDepth) { // add reflections

		glm::vec4 curN = intersectPt.getNormalWorldSpace();
		glm::vec4 v = -intersectPt.d;
		glm::vec4 nextD = glm::normalize(2.0f * curN * glm::dot(curN, v) - v);

		// prevent self intersection
		glm::vec4 nextEye = intersectPt.getPointOfIntersectionWorldSpace() + m_epsilon * nextD;

		IntersectPoint nextIntersectPt;
		bool intersectionExists = getClosestIntersection(&nextIntersectPt, nextD, nextEye);

		if (intersectionExists) {
			glm::vec4 reflectiveIntensity = getColor(intersectPt.getPointOfIntersectionWorldSpace(), nextIntersectPt, depth + 1);
			glm::vec4 reflectiveComponent = m_global.ks * aMaterial.cReflective * reflectiveIntensity;
			finalIntensity += reflectiveComponent;
		}
	}
	
	for (int i = 0; i < 3; i++) {
		finalIntensity[i] = fmax(0.0f, fmin(1.0f, finalIntensity[i]));
	}

	return finalIntensity;
}


glm::vec4 RayScene::getNormalizedD(int j, int i, int W, int H, float thetaH, glm::vec4 eye) {
	GLfloat k = 2.0f;

	// convert to radians
	thetaH = glm::radians(thetaH);
	GLfloat aspectRatio = 1.0f * W / H;
	GLfloat thetaW = thetaH * aspectRatio;

	GLfloat x = (i + 0.5f) / W - 0.5f;
	GLfloat y = 0.5f - (j + 0.5f) / H;

	GLfloat U = 2 * k * glm::tan(thetaW / 2.0f);
	GLfloat V = 2 * k * glm::tan(thetaH / 2.0f);

	glm::vec4 uvk = glm::vec4(U * x, V * y, -k, 1);
	
	glm::vec4 d = uvk - eye;
	d = glm::normalize(d);
	return d;
}

