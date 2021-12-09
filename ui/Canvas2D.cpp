/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"

#include "filter/FilterBlur.h"
#include "filter/FilterEdgeDetect.h"
#include "filter/FilterScale.h"


Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
	m_rayScene(nullptr)
{
	switch(settings.brushType) {
		case BRUSH_CONSTANT: 
			this->m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
			break;
		case BRUSH_LINEAR: 
			this->m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
			break;
		case BRUSH_QUADRATIC: 
			this->m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);
			break;
		case BRUSH_SMUDGE: 
			this->m_brush = std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
			break;
		default: 
			this->m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
	}
	this->m_brush->setBrushType(settings.brushType);
}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;

	// if (settings.brushColor != m_brush->getRGBA()) {
	//     this->m_brush->setRGBA(settings.brushColor);
	// }
	if (settings.brushColor.r != m_brush->getRed()) {
		this->m_brush->setRed(settings.brushColor.r);
	}
	else if (settings.brushColor.g != m_brush->getGreen()) {
		this->m_brush->setGreen(settings.brushColor.g);
	}
	else if (settings.brushColor.b != m_brush->getBlue()) {
		this->m_brush->setBlue(settings.brushColor.b);
	}
	else if (settings.brushColor.a != m_brush->getAlpha()) {
		this->m_brush->setAlpha(settings.brushColor.a);
	}

	else if (settings.brushRadius != m_brush->getRadius()) {
		this->m_brush->setRadius(settings.brushRadius);
	}
	else if (settings.brushType != m_brush->getBrushType()) {
		switch(settings.brushType) {
			case BRUSH_CONSTANT: 
				this->m_brush = std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
				break;
			case BRUSH_LINEAR: 
				this->m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
				break;
			case BRUSH_QUADRATIC: 
				this->m_brush = std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);
				break;
			case BRUSH_SMUDGE: 
				this->m_brush = std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
				break;
			default: 
				this->m_brush = std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
		}
		this->m_brush->setBrushType(settings.brushType);
	}


	// this->m_brush.reset (new ConstantBrush(settings.brushColor, settings.brushRadius));
	// this->m_brush = new ConstantBrush(settings.brushColor, settings.brushRadius);
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    std::cout << "Canvas2d::mouseDown() called" << std::endl;
	m_brush->brushDown(x, y, this);


//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;
	m_brush->brushDragged(x, y, this);
}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseUp() called" << std::endl;
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.
	
	std::unique_ptr<Filter> aFilter;

    switch(settings.filterType) {
		case FILTER_BLUR:
			aFilter = std::make_unique<FilterBlur>();
			break;
		case FILTER_EDGE_DETECT:
			aFilter = std::make_unique<FilterEdgeDetect>();
			break;
		case FILTER_SCALE:
			aFilter = std::make_unique<FilterScale>();
			break;
    }

	aFilter->apply(this);

    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(CS123SceneCameraData *camera, int width, int height) {
    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

		this->resize(width, height);
		RGBA *pix = this->data();

		glm::vec4 w = glm::normalize(-camera->look);
		glm::vec4 v = glm::normalize(camera->up - glm::dot(camera->up, w) * w);
		glm::vec4 u = glm::vec4(glm::cross(glm::vec3(v), glm::vec3(w)), 0);

		glm::mat4 mTranslate = { {1, 0, 0, 0},\
			{0, 1, 0, 0},\ 
			{0, 0, 1, 0},\
			{-camera->pos.x, -camera->pos.y, -camera->pos.z, 1} };

		glm::mat4 mRotate = { {u.x, v.x, w.x, 0},\
			{u.y, v.y, w.y, 0},\
			{u.z, v.z, w.z, 0},\
			{0, 0, 0, 1} };

		glm::mat4x4 camTransformationMatrix = mRotate * mTranslate;
		glm::mat4x4 inverseCamTransformationMatrix = glm::inverse(camTransformationMatrix);

		// store qimages for all objects
		
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				// std::cout << "going through pixels\n";

				// generate rays in camera space
				glm::vec4 pEye = glm::vec4(0, 0, 0, 1);
				glm::vec4 d = m_rayScene->getNormalizedD(j, i, width, height, camera->heightAngle, pEye);	

				// transform rays from camera space to world space
				glm::vec4 worldSpaceD = glm::normalize(inverseCamTransformationMatrix * d);
				glm::vec4 worldSpacePEye = inverseCamTransformationMatrix * pEye;

				// loop through all objects in the scene, and use the one with the minimum t-value
				IntersectPoint intersectPt;
				bool intersectionExists = m_rayScene->getClosestIntersection(&intersectPt, worldSpaceD, worldSpacePEye);

				if (intersectionExists) {
					// object space, convert into world space

					glm::vec4 color = m_rayScene->getColor(camera->pos, intersectPt, 0);
					color *= 255.0f; // convert to from 1.0 to 255.0 for RGBA

					int pixelIndex = j * width + i;
					pix[pixelIndex] = RGBA(color.x, color.y, color.z, 255);
				}
			}

			if (j % 5 == 0) {
				QCoreApplication::processEvents();
				this->update();
			}
		}
		std::cout << "Render complete\n";
		this->update();




        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.
		QCoreApplication::processEvents();

    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




