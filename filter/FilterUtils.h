#ifndef FILTERUTILS_H
#define FILTERUTILS_H

#include "RGBA.h"
#include <vector>
#include <glm/glm.hpp>

namespace FilterUtils {

inline unsigned char REAL2byte(float f);
void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel);

bool IsWithinBounds(int curRow, int curCol, int height, int width);
RGBA vec4toRGBA(glm::vec4 val);

void Convolve1D(\
		const std::vector<glm::vec4> &ref, \
		std::vector<glm::vec4> &results, \
		int width, int height, \
		std::vector<float> aKernel, bool isVertical);

void ConvolveSeparate2D(const std::vector<glm::vec4> &pixFloat, \
		std::vector<glm::vec4> &results, \
		int width, int height, \
		std::vector<float> &rowKernel, \
		std::vector<float> &colKernel);
}

#endif // FILTERUTILS_H
