#include "FilterGray.h"

#include <iostream>

unsigned char RGBAToGray(const RGBA &pixel) {
    // TODO: Task 5
	unsigned char y = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
	return y;
}

FilterGray::~FilterGray()
{
}

void FilterGray::apply(Canvas2D *canvas) {
	std::cout << "applying greyscale filter" << std::endl;
    int width = canvas->width();
    int height = canvas->height();

    RGBA* current_pixel = nullptr;

    /* Initialize the first pixel of the first row */
    RGBA* current_row = canvas->data();

    RGBA* data = canvas->data();
    size_t currentIndex = 0;

    for (int r = 0; r < height; r++) {
        current_pixel = current_row;
        currentIndex = r * width;

        for (int c = 0; c < width; c++) {
            // TODO: Task 4
			unsigned char grayVal = RGBAToGray(*current_pixel);

            // TODO: Task 6
			current_pixel->r = grayVal;
			current_pixel->g = grayVal;
			current_pixel->b = grayVal;


            /* Advance to the next pixel */
            current_pixel++;
            currentIndex++;
        }
        current_row += width;
    }
}

