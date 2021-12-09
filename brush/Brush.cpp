/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"

#include <stdio.h>
#include <iostream>

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

int Brush::getBrushType() const {
    return m_brushType;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}

void Brush::setBrushType(int brushType) {
    m_brushType = brushType;
}

void Brush::printMask() {

	std::cout << "-----------printing mask------------" << std::endl;
	std::cout << "radius: " << getRadius()  << std::endl;

	int counter = 0;
	for (int rowCounter = -getRadius(); rowCounter <= getRadius(); rowCounter++) {
		for (int colCounter = -getRadius(); colCounter <= getRadius(); colCounter++) {
			std::cout << m_mask[counter] << " ";
			counter++;
		}
		std::cout << std::endl;
	}
	// std::cout << "-----------------------" << std::endl;
	// for (int i = 0; i <= getRadius() * 2; i++) {
	//     for (int j = 0; j <= getRadius() * 2; j++) {
	//         std::cout << m_mask[i * (getRadius() * 2 + 1) + j] << " ";
	//     }
	//     std::cout << std::endl;
	// }
	std::cout << "-----------end of print mask------------" << std::endl;
}



void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //
	
    RGBA *pix = canvas->data();

	int rowStart = std::max(0, mouseY - m_radius);
	int rowEnd = std::min(canvas->height(), mouseY + m_radius + 1);
	int colStart = std::max(0, mouseX - m_radius);
	int colEnd = std::min(canvas->width(), mouseX + m_radius + 1);
	
	int maskRowStart = std::max(m_radius - mouseY, 0);
	int maskColStart = std::max(m_radius - mouseX, 0);

	for (int rowCounter = rowStart, maskRow = maskRowStart; 
			rowCounter < rowEnd; rowCounter++, maskRow++) {
		for (int colCounter = colStart, maskCol = maskColStart; 
				colCounter < colEnd; colCounter++, maskCol++) {

			int maskPixelIndex = maskRow * (getRadius() * 2 + 1) + (maskCol);
			float alphaPercentage = this->getAlpha() / 255.0f;
			RGBA canvasColor = pix[rowCounter * canvas->width() + colCounter];
			RGBA pixelColor = RGBA(
					lerp(canvasColor.r, this->getRed(),  m_mask[maskPixelIndex] * alphaPercentage), 
					lerp(canvasColor.g, this->getGreen(), m_mask[maskPixelIndex] * alphaPercentage), 
					lerp(canvasColor.b, this->getBlue(),  m_mask[maskPixelIndex] * alphaPercentage), 
					255);

			pix[rowCounter * canvas->width() + colCounter] = pixelColor;
		}
	}

    canvas->update();
}
