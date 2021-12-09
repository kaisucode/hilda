#include "FilterEdgeDetect.h"
#include "Settings.h"

#include "glm/gtx/string_cast.hpp"

#include <iostream>

FilterEdgeDetect::~FilterEdgeDetect()
{
}

void FilterEdgeDetect::apply(Canvas2D *canvas) {

	// apply grayscale filter
	FilterGray::apply(canvas);

	int imageSize = canvas->width() * canvas->height();
	// RGBA result[imageSize];
	RGBA *result = new RGBA[imageSize];

	std::vector<glm::vec4> pixFloat = getCanvasPixFloat(canvas);
	std::vector<glm::vec4> gx(imageSize, glm::vec4(0));
	std::vector<glm::vec4> gy(imageSize, glm::vec4(0));

	// convolve by 1D filter for G_x and G_y
	FilterUtils::ConvolveSeparate2D(pixFloat, gx, canvas->width(), canvas->height(), m_rowKernel, m_colKernel);
	FilterUtils::ConvolveSeparate2D(pixFloat, gy, canvas->width(), canvas->height(), m_colKernel, m_rowKernelReverse);
	
	for (int i = 0; i < imageSize; i++) {
		// take the square root of G_x^2 and G_y^2 to get G
		glm::vec4 val = glm::sqrt(gx.at(i) * gx.at(i) + gy.at(i) * gy.at(i));

		// multiply G by a filter sensitivity parameter p in (0, 1]
		val *= settings.edgeDetectSensitivity;

		// cap values greater than 1 to 1
		val = glm::min(glm::vec4(1.0f), glm::max(glm::vec4(0.0f), val));

		result[i] = FilterUtils::vec4toRGBA(val);
	}

	RGBA *pix = canvas->data();
    memcpy(pix, result, (sizeof(RGBA) * imageSize));
	delete[] result;
}

