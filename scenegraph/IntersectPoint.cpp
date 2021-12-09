#include "IntersectPoint.h"
// #include "Settings.h"
// #include "CS123SceneData.h"

// #include <iostream>

IntersectPoint::IntersectPoint()
{
	this->isValid = false;
}

IntersectPoint::IntersectPoint(glm::vec4 p, glm::vec4 d, GLfloat t, glm::vec4 n, TransformedPrimitive transformedPrimitive)
{
	this->p = p;
	this->d = d;
	this->t = t;
	this->n = n;
	this->transformedPrimitive = transformedPrimitive;
	this->isValid = true;
	this->uv = glm::vec2(-1);
}

IntersectPoint::IntersectPoint(glm::vec4 p, glm::vec4 d, GLfloat t, glm::vec4 n, TransformedPrimitive transformedPrimitive, glm::vec2 uv)
{
	this->p = p;
	this->d = d;
	this->t = t;
	this->n = n;
	this->transformedPrimitive = transformedPrimitive;
	this->isValid = true;
	this->uv = uv;
}

glm::vec4 IntersectPoint::getPointOfIntersection() {
	return p + d * t;
}

glm::vec4 IntersectPoint::getPointOfIntersectionWorldSpace() {
	return this->transformedPrimitive.transformations * this->getPointOfIntersection();
}

glm::vec4 IntersectPoint::getNormalWorldSpace() {
	glm::mat3 transposedInverse = glm::inverse(glm::transpose(glm::mat3(transformedPrimitive.transformations)));
	glm::vec3 normWorldSpace = glm::normalize(transposedInverse * glm::vec3(this->n));
	return glm::vec4(normWorldSpace.x, normWorldSpace.y, normWorldSpace.z, 0);
}

glm::vec2 IntersectPoint::getST() {
	GLfloat j = transformedPrimitive.primitive.material.textureMap.repeatU;
	GLfloat k = transformedPrimitive.primitive.material.textureMap.repeatV;
	int w = transformedPrimitive.textureImage.width();
	int h = transformedPrimitive.textureImage.height();
	
	float u = uv[0];
	float v = uv[1];

	// int s = ((int) (u * j * w)) % w;
	// int t = ((int) (v * k * h)) % h;

	int s = std::floor((u * j - std::floor(j * u)) * w);
	int t = std::floor((v * k - std::floor(k * v)) * h);
	return glm::vec2(s, t);
}

glm::vec4 IntersectPoint::getTextureColor() {
	
	CS123SceneMaterial aMaterial = transformedPrimitive.primitive.material;

	glm::vec2 st = getST();
	QRgb textureColor = transformedPrimitive.textureImage.pixel(st[0], st[1]);
	glm::vec4 cTexture(qRed(textureColor), qGreen(textureColor), qBlue(textureColor), 0);

	return cTexture;
}

IntersectPoint::~IntersectPoint()
{
}

