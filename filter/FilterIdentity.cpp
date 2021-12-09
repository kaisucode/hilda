#include "FilterIdentity.h"
#include "FilterUtils.h"

FilterIdentity::FilterIdentity()
{
    // TODO: Task 17 Initialize kernel for identity

	m_kernel = { 
		0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f 
	};
}

FilterIdentity::~FilterIdentity()
{
}

void FilterIdentity::apply(Canvas2D *canvas) {
    // TODO: Task 16 Call convolve function from superclass

	RGBA *pix = canvas->data();
	FilterUtils::Convolve2D(pix, canvas->width(), canvas->height(), m_kernel);

}
