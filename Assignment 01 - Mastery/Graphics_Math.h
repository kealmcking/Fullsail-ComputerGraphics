#pragma once
#include <algorithm>

# define M_PI           3.14159265358979323846

// I know many of these functions already exist in the Standard Library, but I thought it would be a fun exercise to make my own
// from scratch.
class Math {
public:

	// Custom lerp function
	float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}

	// Returns delta
	unsigned int delta(int _x1, int _x2) {
		return _x2 - _x1;
	}

	// Uses Diagonal of a Polygon Formula (n * (n-3) / 2) to determine number of diagonal sides
	int diagonals(int shapeSides) {
		int n = shapeSides;
		return n * (n - 3) / 2;
	}

	bool isSideDiagonal(int sides, int diagonals, int index) {
		// If returns a 0, perpendicular. If returns a 1, Diagonal
		return index % 2 == 1;
	}
};
