/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <math.h>
#include <iostream>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...

	for (int rowCounter = -getRadius(); rowCounter <= getRadius(); rowCounter++) {
		for (int colCounter = -getRadius(); colCounter <= getRadius(); colCounter++) {

			// std::cout << "(" << rowCounter << ", " << colCounter << ")";
			// std::cout << (rowCounter * rowCounter + colCounter * colCounter) << " ";
			int distSquared = rowCounter * rowCounter + colCounter * colCounter;
			int maxDist = getRadius() * getRadius();

			// if distSquared >= maxDist
			// TODO

			float distPercentage = 1.0f * distSquared / maxDist;
			this->m_mask.push_back(1.0f - distPercentage);

		}
		// std::cout << std::endl;
	}

	int counter = 0;
	for (int rowCounter = -getRadius(); rowCounter <= getRadius(); rowCounter++) {
		for (int colCounter = -getRadius(); colCounter <= getRadius(); colCounter++) {
			std::cout << m_mask[counter] << " ";
			counter++;
		}
		std::cout << std::endl;
	}
}


