// Diamond Square Algorithm Terrain Generator
// Algorithm implementation by danielbeard
// http://danielbeard.wordpress.com/2010/08/07/terrain-generation-and-smoothing/

#include <cstdlib>
#include <cmath>

#include "Terrain.hpp"

// an initial seed value for the corners of the data
double SEED = 0.0;
double terrainData[DATA_SIZE][DATA_SIZE];

// for the range of values
float maxY = -1e32;
float minY = 1e32;

void generateTerrain(unsigned int seed) {
	// for loop initializers
	int sideLength, x, y, i, j;

	// initialise the values of the corners
	terrainData[0][0] = terrainData[0][DATA_SIZE-1] = terrainData[DATA_SIZE-1][0] = terrainData[DATA_SIZE-1][DATA_SIZE-1] = SEED;

	double h = 200.0; // the range (-h -> h) for the average offset
	srand(seed);      // seed the random generator

	// side length is the distance of a single square side
	// or distance of diagonal in diamond
	// each iteration we are looking at smaller squares and diamonds, we decrease the variation of the offset
	for (sideLength = DATA_SIZE-1; sideLength >= 2; sideLength /= 2, h /= 2.0) {
		int halfSide = sideLength/2;

		// generate new square values
		for(x = 0; x < DATA_SIZE - 1; x += sideLength) {
			for(y = 0; y < DATA_SIZE - 1; y += sideLength) {
				// x, y is upper left corner of the square
				// calculate average of existing corners
				double avg = terrainData[x][y] +         //top   left
				terrainData[x+sideLength][y]   +         //top   right
				terrainData[x][y+sideLength]   +         //lower left
				terrainData[x+sideLength][y+sideLength]; //lower right

				avg /= 4.0;

				// center is average plus random offset in the range (-h, h)
				double offset = (-h) + rand() * (h - (-h)) / RAND_MAX;
				terrainData[x+halfSide][y+halfSide] = avg + offset;

			}
		}

		// Generate the diamond values
		// Since diamonds are staggered, we only move x by half side
		// NOTE: if the data shouldn't wrap the x < DATA_SIZE and y < DATA_SIZE
		for (x = 0; x < DATA_SIZE - 1; x += halfSide) {
			for (y = (x + halfSide) % sideLength; y < DATA_SIZE - 1; y += sideLength) {
				// x,y is center of diamond
				// we must use mod and add DATA_SIZE for subtraction
				// so that we can wrap around the array to find the corners

				double avg =
				terrainData[(x-halfSide+DATA_SIZE)%DATA_SIZE][y] + // left  of center
				terrainData[(x+halfSide)%DATA_SIZE][y]           + // right of center
				terrainData[x][(y+halfSide)%DATA_SIZE]           + // below center
				terrainData[x][(y-halfSide+DATA_SIZE)%DATA_SIZE];  // above center

				avg /= 4.0;

				// new value = average plus random offset
				// calc random value in the range (-h,+h)
				double offset = (-h) + rand() * (h - (-h)) / RAND_MAX;
				avg = avg + offset;

				// update value for center of diamond
				terrainData[x][y] = avg;

				// wrap values on the edges
				// remove this and adjust loop condition above
				// for non-wrapping values
				if (x == 0) terrainData[DATA_SIZE-1][y] = avg;
				if (y == 0) terrainData[x][DATA_SIZE-1] = avg;
			}
		}
	}

	// Calculate minY and maxY values
	for (i = 0; i < DATA_SIZE - 1; i++) {
		for(j = 0; j < DATA_SIZE - 1; j++) {
			if (terrainData[i][j] > maxY)
				maxY = terrainData[i][j];
			if (terrainData[i][j] < minY)
				minY = terrainData[i][j];
		}
	}
}
