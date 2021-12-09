/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>
#include <iostream>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Quadratic brush here...
	
	this->m_mask.clear();
	this->m_mask.reserve((getRadius() * 2 + 1) * (getRadius() * 2 + 1));
	float A = 1.0f / (getRadius() * getRadius());
	float B = -2.0f / getRadius();
	float C = 1.0f;

	for (int rowCounter = -getRadius(); rowCounter <= getRadius(); rowCounter++) {
		for (int colCounter = -getRadius(); colCounter <= getRadius(); colCounter++) {
			int distSquared = rowCounter * rowCounter + colCounter * colCounter;
			int maxDist = getRadius() * getRadius();

			if (distSquared <= maxDist) {
				float paintApplied = A * distSquared + B * sqrt(distSquared) + C;
				this->m_mask.push_back(paintApplied);
			}
			else {
				this->m_mask.push_back(0);
			}
		}
	}
}


