#include "Graphics.h"

// Clear the color buffer to a solid color of my choice
// ADD A COLOR PARAMETER WHEN YOU FIGURE OUT HOW TO DO SO
void Graphics::ClearAndSetColorBuffer() {
	// TODO: CLEAR COLOR BUFFER
	
	// TODO: SET COLOR BUFFER TO COLOR PASSED IN
}

// Converts 2 Dimensional Coordinates to a 1 Dimensional Coordinate
int Graphics::ConvertCoords(int x, int y) {
	return x * y;
}

// Draws a pixel (Fill a certain pixel with a specific color)
// ADD A COLOR PARAMETER WHEN YOU FIGURE OUT HOW TO DO SO
void Graphics::DrawPixel(int xPos, int yPos) {
	// Convert pixel pos into 1d coord
	unsigned int pixel = ConvertCoords(xPos, yPos);
}

/* Perform Block Image Transfer(Research wtf this means so you can
*  implement it / document what it does)
*/
void Graphics::PerformBLIT() {

}
