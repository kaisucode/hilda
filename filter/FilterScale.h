#ifndef FilterScale_Header
#define FilterScale_Header

// #include "FilterGray.h"
#include "Filter.h"
#include "FilterUtils.h"

class FilterScale
	: public Filter
{
	public:
		virtual ~FilterScale() override;
		virtual void apply(Canvas2D *canvas) override;

		glm::vec4 hPrime(int k, float a, std::vector<glm::vec4> &pixFloat, \
				int row, int col, int width, int height, \
				int newWidth, int newHeight, bool isVertical);

		float g(float x, float a);
};

#endif

