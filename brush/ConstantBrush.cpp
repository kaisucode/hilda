/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"
#include <iostream>

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...

	this->m_mask.clear();
	this->m_mask.reserve((getRadius() * 2 + 1) * (getRadius() * 2 + 1));
	for (int rowCounter = -getRadius(); rowCounter <= getRadius(); rowCounter++) {
		for (int colCounter = -getRadius(); colCounter <= getRadius(); colCounter++) {

			int distSquared = rowCounter * rowCounter + colCounter * colCounter;
			int maxDist = getRadius() * getRadius();
			if (distSquared <= maxDist) {
				this->m_mask.push_back(1.0f);
			}
			else {
				this->m_mask.push_back(0.0f);
			}
		}
	}
}


