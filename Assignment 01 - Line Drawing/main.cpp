#include <iostream>
#include "include/RasterSurface.h"
#include <Windows.h>
#include <vector>
#include <random>

#define RASTER_HEIGHT 500
#define RASTER_WIDTH 500
#define NUM_PIXELS	(RASTER_WIDTH * RASTER_HEIGHT)

unsigned SCREEN_ARRAY[NUM_PIXELS];

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

	unsigned int srcA = (color >> 24) & 0xFF;
	unsigned int srcR = (color >> 16) & 0xFF;
	unsigned int srcG = (color >> 8) & 0xFF;
	unsigned int srcB = color & 0xFF;

	// If pixel has no alpha data, just set it to the color of the pixel without any conversion
	if (srcA == 255) {
		SCREEN_ARRAY[index] = color;
		return;
	}


	unsigned int destColor = SCREEN_ARRAY[index];
	unsigned int destR = (destColor >> 16) & 0xFF;
	unsigned int destG = (destColor >> 8) & 0xFF;
	unsigned int destB = destColor & 0xFF;

	float alpha = srcA / 255.0f;
	unsigned int blendedR = static_cast<unsigned int>(srcR * alpha + destR * (1 - alpha));
	unsigned int blendedG = static_cast<unsigned int>(srcG * alpha + destG * (1 - alpha));
	unsigned int blendedB = static_cast<unsigned int>(srcB * alpha + destB * (1 - alpha));

	// Fill pixel with specific color
	SCREEN_ARRAY[index] = (0xFF << 24) | (blendedR << 16) | (blendedG << 8) | blendedB;
}

POINT GeneratePoint(int x, int y) {
	POINT point = { x , y };

	return point;
}


int main() {
	RS_Initialize("Keagon King", RASTER_WIDTH, RASTER_HEIGHT);

	do {


	} while (RS_Update(SCREEN_ARRAY, NUM_PIXELS));

	RS_Shutdown();
	return 0;

}

