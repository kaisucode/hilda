#include "Filter.h"

Filter::~Filter()
{
}

std::vector<glm::vec4> Filter::getCanvasPixFloat(Canvas2D *canvas) {
	int imageSize = canvas->width() * canvas->height();

	RGBA *pix = canvas->data();
	std::vector<glm::vec4> pixFloat;
	pixFloat.reserve(imageSize);

	for (int i = 0; i < imageSize; i++) {
		glm::vec4 aPixel(0);
		aPixel[0] = 1.0f * pix[i].r / 255.0f;
		aPixel[1] = 1.0f * pix[i].g / 255.0f;
		aPixel[2] = 1.0f * pix[i].b / 255.0f;
		aPixel[3] = 1.0f * pix[i].a / 255.0f;
		pixFloat.push_back(glm::vec4(aPixel));
	}

	return pixFloat;
}

