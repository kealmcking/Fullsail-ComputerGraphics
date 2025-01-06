#include "RasterSurface.h"
#include "Graphics.h"

int main() {
	RS_Initialize("Keagon King", 500, 500);

	// Create an unsigned int array to hold the pixel data
	const unsigned int totalPixels = 500 * 500;
	unsigned int* pixels = new unsigned int[totalPixels];

	while (RS_Update(pixels, totalPixels)) {
		// TODO: DRAW PIXEL
		
		// TODO: DRAW A TILE / BLIT AN IMAGE

		// TODO: FILL COLOR BUFFER WITH IMAGE
	}

	delete[] pixels;

	RS_Shutdown();
	return 0;

}