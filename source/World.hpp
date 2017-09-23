#pragma once

#include <vector>
#include <grrlib.h>

//Size of the grid to generate
//This must be 2^n+1 (e.g. 257)
#define DATA_SIZE 513

#define worldX 512
#define worldY 64
#define worldZ 512

#define chunkSize 16

extern float maxY;
extern float minY;
extern double terrainData[DATA_SIZE][DATA_SIZE];

class World {
	public:
		uint8_t theWorld[worldY][worldX][worldZ];
		uint8_t lighting[worldX][worldZ];
		std::vector<guVector> flowingLiquid;

		World(unsigned int);
		uint8_t getBlock(int, int, int);
		void setBlockRaw(int, int, int, uint8_t);
		void setBlock(int, int, int, uint8_t);
		void setIfAir(int, int, int, uint8_t);
		int getLiquidsSize();
		int getLiquidsCapacity();

		void updateWorld(int);

		void placeTree(int, int, int);
};
