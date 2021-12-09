#include "FilterBlur.h"
#include "Settings.h"

#include "glm/gtx/string_cast.hpp"

#include <iostream>

FilterBlur::~FilterBlur()
{
}

std::vector<float> FilterBlur::generateGaussianKernel(int blurRadius) {

	int kernelLength = blurRadius * 2 + 1;
	std::vector<float> kernel(kernelLength, 0.0f);

	int mu = 0;
	float sigma = blurRadius / 3.0f;

	float kernelSum = 0.0f;
	float coefficient = 1.0f / (sqrt(2.0f * M_PI) * sigma);
	for (int i = blurRadius; i >= 0; i--) {
		float val = coefficient * exp(-(i * i) / (2 * sigma * sigma));

		int kernelIndex = blurRadius - i;
		kernel.at(kernelIndex) = val;
		kernel.at(kernelLength - kernelIndex - 1) = val;
		kernelSum += val * 2;
	}
	kernelSum -= kernel.at(blurRadius);

	// normalize kernel
	for (int i = 0; i < kernelLength; i++) {
		kernel.at(i) /= kernelSum;
	}

	return kernel;
}

void FilterBlur::apply(Canvas2D *canvas) {

	std::vector<float> gaussianKernel = generateGaussianKernel(settings.blurRadius);

	int imageSize = canvas->width() * canvas->height();
	// RGBA result[imageSize];
	RGBA *result = new RGBA[imageSize];

	std::vector<glm::vec4> pixFloat = getCanvasPixFloat(canvas);
	std::vector<glm::vec4> convolvedPixels(imageSize, glm::vec4(0));

	FilterUtils::ConvolveSeparate2D(pixFloat, convolvedPixels, canvas->width(), canvas->height(), gaussianKernel, gaussianKernel);
	
	for (int i = 0; i < imageSize; i++) {
		glm::vec4 val = convolvedPixels.at(i);

		// cap values greater than 1 to 1
		val = glm::min(glm::vec4(1.0f), glm::max(glm::vec4(0.0f), val));

		result[i] = FilterUtils::vec4toRGBA(val);
	}

	RGBA *pix = canvas->data();
    memcpy(pix, result, (sizeof(RGBA) * imageSize));

	delete[] result;
}

