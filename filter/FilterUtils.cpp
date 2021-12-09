#include "FilterUtils.h"
#include <algorithm>
#include "math.h"
#include <cstring>

#include <iostream>



namespace FilterUtils {

	inline unsigned char REAL2byte(float f) {
		int i = static_cast<int>((f * 255.0 + 0.5));
		return (i < 0) ? 0 : (i > 255) ? 255 : i;
	}

	RGBA vec4toRGBA(glm::vec4 val) {
		return RGBA(REAL2byte(val[0]), REAL2byte(val[1]), REAL2byte(val[2]), 255.f);
	}

	bool IsWithinBounds(int curRow, int curCol, int height, int width) {
		return (curRow < height && curRow >= 0 
				&& curCol < width && curCol >= 0);
	}

	void Convolve1D(\
			const std::vector<glm::vec4> &ref, \
			std::vector<glm::vec4> &results, \
			int width, int height, \
			std::vector<float> aKernel, bool isVertical) {

		int kernelStartDiff = (aKernel.size() - 1) / 2;

		for (int row = 0; row < height; row++) {
			for (int col = 0; col < width; col++) {

				glm::vec4 sum(0.0f); 
				for (int kernelIndex = 0; kernelIndex < aKernel.size(); kernelIndex++) { 

					int curRow = row;
					int curCol = col;

					if (!isVertical) {
						curCol = col - kernelStartDiff + kernelIndex;
						curCol = std::min(width - 1, std::max(0, curCol));
					}
					else {
						curRow = row - kernelStartDiff + kernelIndex;
						curRow = std::min(height - 1, std::max(0, curRow));
					}

					int pixelIndex = curRow * width + curCol;
					sum += aKernel[kernelIndex] * ref[pixelIndex]; 
				}

				results.at(row * width + col) = sum;
			}
		}
	}

	void ConvolveSeparate2D(\
			const std::vector<glm::vec4> &pixFloat, \
			std::vector<glm::vec4> &results, \
			int width, int height, \
			std::vector<float> &rowKernel, \
			std::vector<float> &colKernel) {

		int kernelStartDiff;
		std::vector<glm::vec4> tempResults(width * height, glm::vec4(0));

		Convolve1D(pixFloat, tempResults, width, height, rowKernel, false);
		Convolve1D(tempResults, results, width, height, colKernel, true);

	}


	void Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel) {
		// TODO: Task 9 Create buffer to store new image data
		RGBA result[width * height];

		// TODO: Task 10 Obtain kernel dimension
		int kernelSide = sqrt(kernel.size());


		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {
				size_t centerIndex = r * width + c;

				// TODO: Task 11 Initialize color to accumulate convolution data
				float red_acc = 0.0f, green_acc = 0.0f, blue_acc = 0.0f;


				// TODO: Task 12
				// Iterate over the kernel using the value from task 10
				// Obtain the value at current index of kernel
				// Find correct index in original image data
				// Accumulate the kernel applied to pixel value in color_acc

				int pixelRowStart = r - ((kernelSide - 1) / 2);
				int pixelColStart = c - ((kernelSide - 1) / 2);
				for (int i = 0; i < kernelSide; i++) {
					int pixelRow = pixelRowStart + i;

				// TODO: Task 13 Ignore outside bounds
					if (pixelRow >= height || pixelRow < 0) {
						continue;
					}
					for (int j = 0; j < kernelSide; j++) {
						int pixelCol = pixelColStart + j;
						if (pixelCol >= width || pixelCol < 0) {
							continue;
						}

						int kernelIndex = i * kernelSide + j;
						int pixelIndex = pixelRow * width + pixelCol;

						red_acc += data[pixelIndex].r * kernel[kernelIndex];
						blue_acc += data[pixelIndex].g * kernel[kernelIndex];
						green_acc += data[pixelIndex].b * kernel[kernelIndex];
					}
				}



				// TODO: Task 14 Update buffer with accumulated color
				red_acc = red_acc/255.f;
				green_acc = green_acc/255.f;
				blue_acc = blue_acc/255.f;
				int index = r * width + c;
				result[index] = RGBA(REAL2byte(red_acc), REAL2byte(blue_acc), REAL2byte(green_acc), 255.f);
			}
		}

		// TODO: Task 15 Copy over buffer to canvas data
		std::memcpy(data, result, (sizeof(RGBA) * width * height));

	}

}
