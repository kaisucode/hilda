#include "FilterShift.h"
#include "FilterUtils.h"

FilterShift::FilterShift(ShiftDirection direction):
    m_shiftDir(direction)
{
    // TODO: Task 19 Initialize kernel for shift

	if (direction == SHIFT_LEFT) {
		m_kernel = { 
			0.0f, 0.0f, 0.0f, 
			0.0f, 0.0f, 1.0f, 
			0.0f, 0.0f, 0.0f 
		};
	}
	else if (direction == SHIFT_RIGHT) {
		m_kernel = { 
			0.0f, 0.0f, 0.0f, 
			1.0f, 0.0f, 0.0f, 
			0.0f, 0.0f, 0.0f 
		};
	}
	else {
		m_kernel = { 
			0.0f, 0.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 
			0.0f, 0.0f, 0.0f 
		};
	}
}

FilterShift::~FilterShift()
{
}

void FilterShift::apply(Canvas2D *canvas) {
    // TODO: Task 18 Call convolve function from superclass
	RGBA *pix = canvas->data();
	FilterUtils::Convolve2D(pix, canvas->width(), canvas->height(), m_kernel);
	// FilterUtils::Convolve2D(canvas->data(), canvas->width(), canvas->height(), m_kernel);

}
