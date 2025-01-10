#include <iostream>
#include "RasterSurface.h"
#include <Windows.h>
#include "Graphics_Math.h"
#include <vector>
#include <random>

#define RASTER_HEIGHT 500
#define RASTER_WIDTH 500
#define NUM_PIXELS	(RASTER_WIDTH * RASTER_HEIGHT)

unsigned SCREEN_ARRAY[NUM_PIXELS];

Math math;

// Clear the color buffer to a solid color of my choice
void ClearAndSetColorBuffer(unsigned int color) {
	for (int i = 0; i < NUM_PIXELS; i++) {
		SCREEN_ARRAY[i] = color;
	}
}

// Converts 2 Dimensional Coordinates to a 1 Dimensional Coordinate
int ConvertCoords(int x, int y, int nWidth) {
	return y * nWidth + x;
}

// Draws a pixel (Fill a certain pixel with a specific color)
void DrawPixel(unsigned int xPos, unsigned int yPos, unsigned int color) {
	if (xPos >= RASTER_WIDTH || yPos >= RASTER_HEIGHT) {
		return;
	}

	// Convert pixel pos into 1d coord
	unsigned int index = ConvertCoords(xPos, yPos, RASTER_WIDTH);

	// If pixel has no alpha data, just set it to the color of the pixel without any conversion
	SCREEN_ARRAY[index] = color;
}

// TODO : Function for doing the math for the Bresenham Line Algorithm (Mastery Assignment)
void BresenhamLineAlgorithm(unsigned int _x1, unsigned int _y1, unsigned int _x2, unsigned int _y2) {

}

// TODO : Function for doing the math for the Midpoint Line Algorithm (Mastery Assignment)
void MidPointLineAlgorithm(unsigned int _x1, unsigned int _y1, unsigned int _x2, unsigned int _y2) {

}

void ParametricLineAlgorithm(int _x1, int _y1, int _x2, int _y2, unsigned int color) {
	int deltaX = math.delta(_x1, _x2);
	int deltaY = math.delta(_y1, _y2);

	if (deltaX > 0) {
		for (int x = _x1; x <= _x2; x++) {
			float ratio = static_cast<float>(x - _x1) / deltaX;
			int y = math.lerp(_y1, _y2, ratio);
			DrawPixel(x, y, color);
		}
	}
	else if (deltaX < 0) {
		for (int x = _x1; x >= _x2; x--) {
			float ratio = static_cast<float>(x - _x1) / deltaX;
			int y = math.lerp(_y1, _y2, ratio);
			DrawPixel(x, y, color);
		}
	}
	else {
		if (deltaY > 0) {
			for (int y = _y1; y <= _y2; y++) {
				DrawPixel(_x1, y, color);
			}
		}
		else {
			for (int y = _y1; y >= _y2; y--) {
				DrawPixel(_x1, y, color);
			}
		}
	}
}

void CreateShape(int perpendicularLength, int diagonalLength, int startingPointX, int startingPointY) {
	unsigned int pink = 0xFFFF00FFF;
	unsigned int yellow = 0xFFFFFF00;
	unsigned int purple = 0xFF00FFFF;
	unsigned int blue = 0xFF0000FF;
	unsigned int green = 0xFF00FF00;
	int pointZeroX = startingPointX;
	int pointZeroY = startingPointY;

	int pointOneX = pointZeroX + perpendicularLength;
	int pointOneY = pointZeroY;

	int pointTwoX = pointOneX + diagonalLength;
	int pointTwoY = pointOneY + diagonalLength;

	int pointThreeX = pointTwoX;
	int pointThreeY = pointTwoY + perpendicularLength;

	int pointFourX = pointThreeX - diagonalLength;
	int pointFourY = pointThreeY + diagonalLength;

	int pointFiveX = pointFourX - perpendicularLength;
	int pointFiveY = pointFourY ;

	int pointSixX = pointFiveX - diagonalLength;
	int pointSixY = pointFiveY - diagonalLength;

	int pointSevenX = pointSixX ;
	int pointSevenY = pointSixY - perpendicularLength;

	// Lines of Octagon
	/* 01 */ ParametricLineAlgorithm(pointZeroX, pointZeroY, pointOneX, pointOneY, pink);
	/* 02 */ ParametricLineAlgorithm(pointOneX, pointOneY, pointTwoX, pointTwoY, pink);
	/* 03 */ ParametricLineAlgorithm(pointTwoX, pointTwoY, pointThreeX, pointThreeY, pink);
	/* 04 */ ParametricLineAlgorithm(pointThreeX, pointThreeY, pointFourX, pointFourY, pink);
	/* 05 */ ParametricLineAlgorithm(pointFourX, pointFourY, pointFiveX, pointFiveY, pink);
	/* 06 */ ParametricLineAlgorithm(pointFiveX, pointFiveY, pointSixX, pointSixY, pink);
	/* 07 */ ParametricLineAlgorithm(pointSixX, pointSixY, pointSevenX, pointSevenY, pink);
	/* 08 */ ParametricLineAlgorithm(pointSevenX, pointSevenY, pointZeroX, pointZeroY, pink);

	// Connective Lines
	/* 01 */ ParametricLineAlgorithm(pointZeroX, pointZeroY, pointFourX, pointFourY, purple);
	/* 01 */ ParametricLineAlgorithm(pointOneX, pointOneY, pointFiveX, pointFiveY, yellow);
	/* 01 */ ParametricLineAlgorithm(pointTwoX, pointTwoY, pointSixX, pointSixY, blue);
	/* 01 */ ParametricLineAlgorithm(pointThreeX, pointThreeY, pointSevenX, pointSevenY, green);

}





int main() {
	RS_Initialize("Keagon King", RASTER_WIDTH, RASTER_HEIGHT);

	do {
		// Creates the Octagon
		CreateShape(100, 70, 200, 125);

	} while (RS_Update(SCREEN_ARRAY, NUM_PIXELS));

	RS_Shutdown();
	return 0;

}

