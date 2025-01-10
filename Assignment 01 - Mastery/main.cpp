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
void BresenhamLineAlgorithm(unsigned int _x1, unsigned int _y1, unsigned int _x2, unsigned int _y2, unsigned int color) {
	int currentY = _y1;
	int deltaX = math.delta(_x1, _x2);
	int deltaY = math.delta(_y1, _y2);

	float slope = (float)deltaY / (float)deltaX;
	float error = 0;

	for (int x = _x1; x <= _x2; x++) {
		DrawPixel(x, currentY, color);
		error += slope;

		if (error > 0.5) {
			currentY += 1;
			error -= 1.0;
		}
	}
}

// TODO : Function for doing the math for the Midpoint Line Algorithm (Mastery Assignment)
void MidPointLineAlgorithm(unsigned int _x1, unsigned int _y1, unsigned int _x2, unsigned int _y2, unsigned int color) {
	int deltaX = math.delta(_x1, _x2);
	int deltaY = math.delta(_y1, _y2);

	int d = 2 * deltaY - deltaX;

	int incrE = 2 * deltaY;
	int incrNE = 2 * (deltaY - deltaX);

	int currX = _x1; int currY = _y1;

	DrawPixel(currX, currY, color);

	while (currX < _x2) {
		if (d <= 0) {
			d += incrE;
		}
		else {
			d += incrNE;
			currY++;
		}
		currX++;
		DrawPixel(currX, currY, color);
	}
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

void GenerateStarfield(int numStars, unsigned int color) {
	srand(static_cast<unsigned int>(time(0)));

	for (int i = 0; i < numStars; i++) {
		int xPos = rand() % RASTER_WIDTH;
		int yPos = rand() % RASTER_HEIGHT;
		DrawPixel(xPos, yPos, color);
	}
}


int main() {
	RS_Initialize("Keagon King", RASTER_WIDTH, RASTER_HEIGHT);
	
	// Clear Buffer
	ClearAndSetColorBuffer(0x00000000);
	GenerateStarfield(2500, 0xFFFFFFFF);

	do {
		
		if (GetAsyncKeyState(VK_NUMPAD1)) {
			// TODO : If pressed, generate a Bresenham Line between 2 random points. Additionally, the first and last point
			// must be yellow.
			std::cout << "1 Pressed!" << std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD2)) {
			// TODO : If pressed, generate a Midpoint Line between 2 random points. Additionally, the first and last point
			// must be yellow.
			std::cout << "2 Pressed!" << std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD3)) {
			// TODO : If pressed, generate a Parametric Line between 2 random points. Additionally, the first and last point
			// must be yellow.
			std::cout << "3 Pressed!" << std::endl;
		}

		if (GetAsyncKeyState(VK_NUMPAD0)) {
			// If pressed, clear all randomly generated lines
			std::cout << "0 Pressed!" << std::endl;
			ClearAndSetColorBuffer(0x00000000);
			GenerateStarfield(2500, 0xFFFFFFFF);
		}

		// Lines
		/* 01 */ BresenhamLineAlgorithm(0, 100, 499, 400, 0xFFFF0000);
		/* 02 */ MidPointLineAlgorithm(0, 90, 499, 390, 0x000FFF00);
		/* 03 */ ParametricLineAlgorithm(0, 110, 499, 410, 0x0000F0FF);
		

	} while (RS_Update(SCREEN_ARRAY, NUM_PIXELS));

	RS_Shutdown();
	return 0;

}

