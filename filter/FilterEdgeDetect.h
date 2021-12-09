#ifndef FilterEdgeDetect_Header
#define FilterEdgeDetect_Header

#include "FilterGray.h"
#include "FilterUtils.h"

class FilterEdgeDetect 
	: public FilterGray 
{
	public:
		virtual ~FilterEdgeDetect() override;

		virtual void apply(Canvas2D *canvas) override;

		std::vector<float> m_rowKernel = { -1.0f, 0.0f, 1.0f };
		std::vector<float> m_rowKernelReverse = { 1.0f, 0.0f, -1.0f };
		std::vector<float> m_colKernel = { 1.0f, 2.0f, 1.0f };
};

#endif

