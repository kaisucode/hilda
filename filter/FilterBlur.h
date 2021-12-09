#ifndef FilterBlur_Header
#define FilterBlur_Header

#include "Filter.h"
#include "FilterUtils.h"

class FilterBlur 
	: public Filter 
{
	public:
		virtual ~FilterBlur() override;

		virtual void apply(Canvas2D *canvas) override;

		std::vector<float> generateGaussianKernel(int blurRadius);
};

#endif

