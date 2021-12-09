/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"

#include <stdio.h>
#include <iostream>

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}

SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!

    // makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //

	QuadraticBrush::makeMask();
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //

    RGBA *pix = canvas->data();
    int width = canvas->width();
    int height = canvas->height();

	int rowStart = y - this->m_radius;
	int rowEnd = y + this->m_radius;
	int colStart = x - this->m_radius;
	int colEnd = x + this->m_radius;

	this->m_paint.clear();
	this->m_mask.reserve((rowEnd - rowStart + 1) * (colEnd - colStart + 1));
	for (int rowCounter = rowStart; rowCounter <= rowEnd; rowCounter++) {
		for (int colCounter = colStart; colCounter <= colEnd; colCounter++) {
			if (rowCounter < 0 || rowCounter > height || colCounter < 0 || colCounter > width) {
				this->m_paint.push_back(RGBA(0, 0, 0, 255));
			}
			else {
				this->m_paint.push_back(pix[rowCounter * width + colCounter]);
			}
		}
	}
}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

	// If no paint is stored, pick up paint and return
	if (m_paint.size() == 0) {
		pickUpPaint(mouseX, mouseY, canvas);
		return;
	}

    RGBA *pix = canvas->data();

	int rowStart = std::max(0, mouseY - m_radius);
	int rowEnd = std::min(canvas->height(), mouseY + m_radius + 1);
	int colStart = std::max(0, mouseX - m_radius);
	int colEnd = std::min(canvas->width(), mouseX + m_radius + 1);

	int maskRowStart = std::max(m_radius - mouseY, 0);
	int maskColStart = std::max(m_radius - mouseX, 0);

	for (int rowCounter = rowStart, maskRow = maskRowStart; 
			rowCounter < rowEnd; 
			rowCounter++, maskRow++) {
		for (int colCounter = colStart, maskCol = maskColStart; 
				colCounter < colEnd; 
				colCounter++, maskCol++) {

			int maskPixelIndex = maskRow * (m_radius * 2 + 1) + (maskCol);

			RGBA canvasColor = pix[rowCounter * canvas->width() + colCounter];
			RGBA brushColor = this->m_paint[maskPixelIndex];
			float alphaPercentage = this->getAlpha() / 255.0f;

			RGBA pixelColor = RGBA(
					lerp(canvasColor.r, brushColor.r, m_mask[maskPixelIndex] * alphaPercentage), 
					lerp(canvasColor.g, brushColor.g, m_mask[maskPixelIndex] * alphaPercentage), 
					lerp(canvasColor.b, brushColor.b, m_mask[maskPixelIndex] * alphaPercentage), 
					255);

			pix[rowCounter * canvas->width() + colCounter] = pixelColor;
		}
	}

    // now pick up paint again...
    pickUpPaint(mouseX, mouseY, canvas);
}


