#pragma once
class Graphics
{
public:
	int ConvertCoords(int x, int y);
	
	void ClearAndSetColorBuffer();
	void DrawPixel(int xPos, int yPos);
	void PerformBLIT();
};

