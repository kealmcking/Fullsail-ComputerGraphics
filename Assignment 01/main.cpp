#include <iostream>
#include "RasterSurface.h"
#include "XTime.h"
#include "tiles_12.h"
#include "fire_02.h"
#include <Windows.h>
#include <vector>
#include <random>

#define RASTER_HEIGHT 500
#define RASTER_WIDTH 500
#define NUM_PIXELS	(RASTER_WIDTH * RASTER_HEIGHT)

unsigned SCREEN_ARRAY[NUM_PIXELS];

XTime xTime;

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

unsigned BGRAtoARGB(unsigned C) {
	// Extracts each channel
	unsigned int b = C & 0xFF;
	unsigned int g = (C >> 8) & 0xFF;
	unsigned int r = (C >> 16) & 0xFF;
	unsigned int a = (C >> 24) & 0xFF;

	// Shifts channels to the correct positions for ARGB and combines them
	return (b << 24) | (g << 16) | (r << 8) | a;
}

// Perform Block Image Transfer
void PerformBLIT(int sourceRectLeft, int sourceRectTop, int sourceRectRightOffset, int sourceRectBottomOffset, int rasterPosX, int rasterPosY, int sourceTextureWidth,const unsigned int* pSourceTextureArray) {
	RECT sourceRect = { sourceRectLeft, sourceRectTop, sourceRectLeft + sourceRectRightOffset, sourceRectTop + sourceRectBottomOffset };


	POINT rasterPos = { rasterPosX, rasterPosY };
	rasterPos.x = rasterPosX;
	rasterPos.y = rasterPosY;

	int rectWidth = sourceRect.right - sourceRect.left;
	int rectHeight = sourceRect.bottom - sourceRect.top;

	for (int y = 0; y < rectHeight; y++) {
		for (int x = 0; x < rectWidth; x++) {
			unsigned int destX = rasterPos.x + x;
			unsigned int destY = rasterPos.y + y;

			// Dont draw pixels outside of the window
			if (destX >= RASTER_WIDTH || destY >= RASTER_HEIGHT) continue;

			// copy a pixel from pSourceTextureArray to the Raster (?)
			unsigned int index = (sourceRect.top + y) * sourceTextureWidth + (sourceRect.left + x);



			unsigned int correctedColor = BGRAtoARGB(pSourceTextureArray[index]);

			DrawPixel(destX, destY, correctedColor);
		}
	}
}


void PlayAnimation(const unsigned int srcImgWidth, const unsigned int srcImgHeight, const unsigned int* srcImgPixels, int framesOfAnimationX, int framesOfAnimationY, float deltaTime, int posX, int posY) {
	int frameWidth = srcImgWidth / framesOfAnimationX;
	int frameHeight = srcImgHeight / framesOfAnimationY;

	int totalFramesOfAnimation = framesOfAnimationX * framesOfAnimationY;


	static int currentFrame = 0;
	static float timeSinceLastFrame = 0.0f;
	const float frameDuration = 1.0f / 30.0f;

	timeSinceLastFrame += deltaTime;
	
	if (timeSinceLastFrame >= frameDuration) {
		timeSinceLastFrame -= frameDuration;
		currentFrame = (currentFrame + 1) % totalFramesOfAnimation;
	}

	int left = (currentFrame % (srcImgWidth / frameWidth)) * frameWidth;
	int top = (currentFrame / (srcImgWidth / frameWidth)) * frameHeight;

	PerformBLIT(left, top, frameWidth, frameHeight, posX, posY, srcImgWidth, srcImgPixels);

}

POINT GeneratePoint(int x, int y) {
	POINT point = { x , y };

	return point;
}

std::vector<POINT> generateRandomPoints() {
	std::vector<POINT> randomPoints;

	// The random seed generator for the positions
	std::random_device device;
	std::mt19937 rng(device());

	std::uniform_int_distribution<int> distX(0, RASTER_WIDTH);
	std::uniform_int_distribution<int> distY(0, RASTER_HEIGHT);

	// Actually creates the random points
	for (int i = 0; i < 10; i++) {
		POINT point = GeneratePoint(distX(rng), distY(rng));

		randomPoints.push_back(point);
	}

	return randomPoints;
}


int main() {
	RS_Initialize("Keagon King", RASTER_WIDTH, RASTER_HEIGHT);
	
	// Stores the random points
	std::vector<POINT> randomPoints = generateRandomPoints();


	do {
		xTime.Signal();
		ClearAndSetColorBuffer(0x000000FF);

		float deltaTime = static_cast<float>(xTime.Delta());

		// Tiles background tile
		for (int x = 0; x < RASTER_WIDTH; x += 30) {
			for (int y = 0; y < RASTER_HEIGHT; y += 30) {
				PerformBLIT(289, 129, 30, 30, x, y, tiles_12_width, tiles_12_pixels);
			}
		}

		// Places objects around at random
		for (int x = 0; x < randomPoints.size();x++) {
			PerformBLIT(320, 14, 61, 80, randomPoints[x].x, randomPoints[x].y, tiles_12_width, tiles_12_pixels);
		}

		// Plays animation
		PlayAnimation(fire_02_width, fire_02_height, fire_02_pixels, 8, 8, deltaTime, 300, 300);

	} while (RS_Update(SCREEN_ARRAY, NUM_PIXELS));

	RS_Shutdown();
	return 0;

}

