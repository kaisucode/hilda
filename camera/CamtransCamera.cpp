/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include <glm/gtx/transform.hpp>

CamtransCamera::CamtransCamera() :
    m_moveSpeed(0.1),
    m_rotateSpeed(5)
{
    // @TODO: [CAMTRANS] Fill this in...
	m_near = 1;
	m_far = 30;
    setHeightAngle(60.0);
	setAspectRatio(1);

	m_eye = glm::vec4(2, 2, 2, 1);
	m_look = glm::vec4(-1.f, -1.f, -1.f, 0.f);
	m_up = glm::vec4(0, 1, 0, 0);
	orientLook(m_eye, m_look, m_up);
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
	m_aspectRatio = a;
    setHeightAngle(glm::degrees(m_thetaH));
	return;
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    // return glm::vec4();
	return -m_w;
}

glm::vec4 CamtransCamera::getU() const {
	return m_u;
}

glm::vec4 CamtransCamera::getV() const {
	return m_v;
}

glm::vec4 CamtransCamera::getW() const {
	return m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    // return glm::vec4();
	return m_up;
}


float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
	return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
	m_eye = eye;
	m_look = look;
	m_up = up;
	// recalculate u, v, w
	m_w = -glm::normalize(m_look);
	m_v = glm::normalize(up - glm::dot(m_w, up) * m_w);
	m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0);

	updateViewMatrix();
	updateProjectionMatrix();
	return;
}

void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...
	m_thetaH = glm::radians(h);
	m_thetaW = glm::atan(m_aspectRatio * glm::tan(m_thetaH / 2.0)) * 2;

	updateProjectionMatrix();
	return;
}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
	m_eye = m_eye + v;
	updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
	
	// apply cos/sin matrices to the ones that are not being rotated about
    glm::mat4 rotation = glm::rotate(glm::radians(degrees), glm::vec3(m_u));
    orientLook(m_eye, rotation * m_look, rotation * m_up);
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::mat4 rotation = glm::rotate(glm::radians(degrees), glm::vec3(m_v));
    orientLook(m_eye, rotation * m_look, rotation * m_up);
}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::mat4 rotation = glm::rotate(glm::radians(degrees), glm::vec3(m_w));
    orientLook(m_eye, rotation * m_look, rotation * m_up);
}

void CamtransCamera::rotateY(float degrees) {
    glm::mat4 rotation = glm::rotate(glm::radians(degrees), glm::vec3(0, 1, 0));
    orientLook(m_eye, rotation * m_look, rotation * m_up);
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
	m_near = nearPlane;
	m_far = farPlane;
	updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix() {
	updateScaleMatrix();
	updatePerspectiveMatrix();
	return;
}

void CamtransCamera::updatePerspectiveMatrix() {
	float c = -m_near / m_far;
	m_perspectiveTransformation = {
		{ 1, 0, 0, 0 },\
		{ 0, 1, 0, 0 },\
		{ 0, 0, -1.0 / (1.0 + c), c / (1.0 + c) },\
		{ 0, 0, -1, 0 }
	};
	m_perspectiveTransformation = glm::transpose(m_perspectiveTransformation);
	return;
}

void CamtransCamera::updateScaleMatrix() {
	m_scaleMatrix = { \
		{ 1.0 / (m_far * glm::tan(m_thetaW / 2)), 0, 0, 0 }, \
		{ 0, 1.0 / (m_far * glm::tan(m_thetaH / 2)), 0, 0 }, \
		{ 0, 0, 1.0 / m_far, 0 }, \
		{ 0, 0, 0, 1.0 }
	};
	return;
}

void CamtransCamera::updateViewMatrix() {
	updateRotationMatrix();
	updateTranslationMatrix();
	return;
}

void CamtransCamera::updateRotationMatrix() {
	m_rotationMatrix = {
		glm::vec4(m_u),\
		glm::vec4(m_v),\
		glm::vec4(m_w),\
		glm::vec4(0, 0, 0, 1)
	};
	m_rotationMatrix = glm::transpose(m_rotationMatrix);
	return;
}

void CamtransCamera::updateTranslationMatrix() {
	m_translationMatrix =  {\
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(glm::vec3(-m_eye), 1)
	};
	return;
}

void CamtransCamera::keyPressed(int key) {
    glm::vec4 dir(0);

    switch (key) {
    case Qt::Key_W:
        dir = glm::vec4(m_look.x, 0, m_look.z, 0);
        break;
    case Qt::Key_A:
        dir = glm::vec4(m_look.z, 0, -m_look.x, 0);
        break;
    case Qt::Key_S:
        dir = glm::vec4(-m_look.x, 0, -m_look.z, 0);
        break;
    case Qt::Key_D:
        dir = glm::vec4(-m_look.z, 0, m_look.x, 0);
        break;
    case Qt::Key_Space:
        dir = glm::vec4(0, 1, 0, 0);
        break;
    case Qt::Key_C:
        dir = glm::vec4(0, -1, 0, 0);
        break;
    case Qt::Key_Up:
        rotateU(m_rotateSpeed);
        break;
    case Qt::Key_Down:
        rotateU(-m_rotateSpeed);
        break;
    case Qt::Key_Right:
        rotateY(-m_rotateSpeed);
        break;
    case Qt::Key_Left:
        rotateY(m_rotateSpeed);
        break;
    }

    if (glm::length(dir) > 0) {
        m_eye += m_moveSpeed * glm::normalize(dir);
    }

    orientLook(m_eye, m_look, m_up);
}
