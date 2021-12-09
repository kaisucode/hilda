#include "FilterScale.h"
#include "Settings.h"

#include <iostream>

FilterScale::~FilterScale()
{
}

float FilterScale::g(float x, float a) {
	float radius = 1.0f;

	if (a < 1) {
		radius = 1.0f / a;
	}
	
	if ((x < -radius) || (x > radius)) {
		return 0;
	}
	else {
		return (1 - fabs(x) / radius) / radius;
	}
}


glm::vec4 FilterScale::hPrime(int k, float a, std::vector<glm::vec4> &pixFloat, \
		int row, int col, int width, int height, int newWidth, int newHeight, bool isVertical) {
	glm::vec4 sum(0.0f);
	float weightsSum(0.0f);

	float center = k / a + (1 - a) / (2 * a);
	float support = (a > 1) ? 1 : (1 / a);
	int left = ceil(center - support);
	int right = floor(center + support);

	// bounds check
	if (!isVertical) {
		left = std::min(width, std::max(0, left));
		right = std::min(width, std::max(0, right));
	}
	else {
		left = std::min(height, std::max(0, left));
		right = std::min(height, std::max(0, right));
	}
	
	for (int i = left; i <= right; i++) {
		int pixIndex = (isVertical) ? (width * i + col) : (width * row + i);

		if (pixIndex >= pixFloat.size()) {
			continue;
		}
		float gValue = g(i - center, a);
		sum += gValue * pixFloat.at(pixIndex);
		weightsSum += gValue;
	}
	return (sum / weightsSum);
}

void FilterScale::apply(Canvas2D *canvas) {

	int width = canvas->width();
	int height = canvas->height();

	int imageSize = width * height;
	int newWidth = width * settings.scaleX;
	int newHeight = height * settings.scaleY;
	int maxImageSize = std::max(width, newWidth) * std::max(height, newHeight);
	// RGBA result[maxImageSize];
	RGBA *result = new RGBA[maxImageSize];

	std::vector<glm::vec4> pixFloat = getCanvasPixFloat(canvas);
	std::vector<glm::vec4> newPixFloat(maxImageSize, glm::vec4(0));

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < newWidth; c++) {
			glm::vec4 val = hPrime(c, settings.scaleX, pixFloat, \
					r, c, width, height, newWidth, newHeight, false);
			newPixFloat.at(r * newWidth + c) = val;
			// result[r * newWidth + c] = FilterUtils::vec4toRGBA(val);
		}
	}

	for (int r = 0; r < newHeight; r++) {
		for (int c = 0; c < newWidth; c++) {
			glm::vec4 val = hPrime(r, settings.scaleX, newPixFloat, \
					r, c, newWidth, height, newWidth, newHeight, true);
			result[r * newWidth + c] = FilterUtils::vec4toRGBA(val);
		}
	}

	canvas->resize(newWidth, newHeight);
	RGBA *pix = canvas->data();
    memcpy(pix, result, (sizeof(RGBA) * newWidth * newHeight));
	delete[] result;
}

