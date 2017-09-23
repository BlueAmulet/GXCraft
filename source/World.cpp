#include <cstdlib>
#include <cmath>
#include <cstdint>

#include "Main.hpp"
#include "World.hpp"
#include "ChunkedRender.hpp"

// an initial seed value for the corners of the data
double SEED = 0.0;
double terrainData[DATA_SIZE][DATA_SIZE];

// for the range of values
float maxY = -1e32;
float minY = 1e32;

static int randnum(int x, int y) {
	double uni = ((double)rand())/((double)RAND_MAX);
	return floor(uni * (y - x + 1)) + x;
}

static void updateNeighbors(int x, int z) {
	if (x % chunkSize == chunkSize-1) Chunked::markchunkforupdate(floor(x/chunkSize)+1,floor(z/chunkSize));
	if (x % chunkSize == 0)           Chunked::markchunkforupdate(floor(x/chunkSize)-1,floor(z/chunkSize));
	if (z % chunkSize == chunkSize-1) Chunked::markchunkforupdate(floor(x/chunkSize),floor(z/chunkSize)+1);
	if (z % chunkSize == 0)           Chunked::markchunkforupdate(floor(x/chunkSize),floor(z/chunkSize)-1);
}

// Diamond Square Algorithm Terrain Generator
// Algorithm implementation by danielbeard
// http://danielbeard.wordpress.com/2010/08/07/terrain-generation-and-smoothing/
static void generateTerrain(unsigned int seed) {
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

World::World(unsigned int seed) {
	flowingLiquid.reserve(32);
	memset(theWorld, 255, sizeof theWorld);

	generateTerrain(seed);
	short x,y,z;
	for (x = 0; x < worldX; x++) {
		for (z = 0; z < worldZ; z++) {
			double terrainPiece = floor(((terrainData[x][z] - minY) / (maxY - minY) * (worldY - 2)) + 1);
			if (terrainPiece < 15)
				lighting[x][z] = 15;
			else
				lighting[x][z] = terrainPiece;
			for (y = 0; y < worldY; y++) {
				if (y == 0)
					theWorld[y][x][z] = 7;
				else if (y < terrainPiece - 2)
					theWorld[y][x][z] = 1;
				else if (y < terrainPiece && y >= terrainPiece - 2)
					theWorld[y][x][z] = 3;
				else if (y == terrainPiece) {
					if (terrainPiece < 15)
						theWorld[y][x][z] = 3;
					else
						theWorld[y][x][z] = 2;
				} else if (y == terrainPiece + 1 && terrainPiece >= 16) {
					int type = rand() % 1000;
					if (type == 1)
						placeTree(x,y,z);
					else
						theWorld[y][x][z] = 0;
				} else if (theWorld[y][x][z] == 255) {
					if (y < 16)
						theWorld[y][x][z] = 8;
					else
						theWorld[y][x][z] = 0;
				}
			}
		}
	}
	// TODO: Add Flower pockets	
}

// Safe block retrieval
uint8_t World::getBlock(int x, int y, int z) {
	if (x >= 0 && x < worldX && y >= 0 && y < worldY && z >= 0 && z < worldZ)
		return theWorld[y][x][z];
	return 0;
}


#define tryAddLiquidBlock(xpos, ypos, zpos) \
blockID = getBlock(xpos, ypos, zpos);\
if (blockID == 8 || blockID == 10) {\
	toAdd.x = xpos;\
	toAdd.y = ypos;\
	toAdd.z = zpos;\
	flowingLiquid.push_back(toAdd);\
}

// Safe block placement
void World::setBlockRaw(int x, int y, int z, uint8_t blockID) {
	if (x >= 0 && x < worldX && y >= 0 && y < worldY && z >= 0 && z < worldZ) {
		theWorld[y][x][z] = blockID;
		if (blockID == 6 || blockID == 18 || blockID == 20 || blockID == 37 || blockID == 38 || blockID == 39 || blockID == 40)
			return;
		guVector toAdd;
		if (blockID != 0) {
			if (lighting[x][z] < y)
				lighting[x][z] = y;
		} else {
			if (y >= lighting[x][z]) {
				int sy;
				for (sy = y - 1; y >= 0; sy--) {
					if (theWorld[sy][x][z] != 0 && theWorld[sy][x][z] != 6 && theWorld[sy][x][z] != 18 && theWorld[sy][x][z] != 20 && theWorld[sy][x][z] != 37 && theWorld[sy][x][z] != 38 && theWorld[sy][x][z] != 39 && theWorld[sy][x][z] != 40) {
						lighting[x][z] = sy;
						break;
					}
				}
			}
			tryAddLiquidBlock(x-1, y, z);
			tryAddLiquidBlock(x+1, y, z);
			tryAddLiquidBlock(x, y, z-1);
			tryAddLiquidBlock(x, y, z+1);
		}
		tryAddLiquidBlock(x, y+1, z);
	}
}

void World::setBlock(int x, int y, int z, uint8_t blockID) {
	this->setBlockRaw(x, y, z, blockID);
	Chunked::markchunkforupdate(floor(x/chunkSize), floor(z/chunkSize));
	updateNeighbors(x, z);
}

void World::setIfAir(int x, int y, int z, uint8_t blockID) {
	u8 tBlockID = this->getBlock(x,y,z);
	if (tBlockID == 0 || tBlockID == 255)
		this->setBlockRaw(x,y,z,blockID);
}

#undef tryAddLiquidBlock

int World::getLiquidsSize() {
	return flowingLiquid.size();
}

int World::getLiquidsCapacity() {
	return flowingLiquid.capacity();
}

#define trySetLiquidBlock(xpos,ypos,zpos) \
if (this->getBlock(liquid.xpos, liquid.ypos, liquid.zpos) == 0) {\
	this->setBlock(liquid.xpos, liquid.ypos, liquid.zpos, liquidType);\
	newLiquid.x = liquid.xpos;\
	newLiquid.y = liquid.ypos;\
	newLiquid.z = liquid.zpos;\
	flowingLiquid.push_back(newLiquid);\
}

void World::updateWorld(int renderDistance) {
	// Random Block Update!
	for (int i = 0; i < pow(renderDistance/16,2)*48; i++) {
		signed int rx = randnum(thePlayer.posX - renderDistance, thePlayer.posX + renderDistance);
		signed int ry = randnum(0, worldY - 1);
		signed int rz = randnum(thePlayer.posZ - renderDistance, thePlayer.posZ + renderDistance);
		u8 blockID = this->getBlock(rx,ry,rz);
		if (blockID == 3 && this->lighting[rx][rz] <= ry) {
			this->setBlock(rx,ry,rz,2);
		} else if (blockID == 2 && this->lighting[rx][rz] > ry) {
			this->setBlock(rx,ry,rz,3);
		} else if (blockID == 6) {
			if (this->lighting[rx][rz] > ry) { //not working?
				this->setBlock(rx,ry,rz,0);
			} else {
				placeTree(rx,ry,rz);
				// this wont update chunks properly. duplicated because we need special chunk care here.
				Chunked::markchunkforupdate(floor(rx/chunkSize), floor(rz/chunkSize));
				updateNeighbors(rx, rz);
			}
		}
	}

	// Fluid Updates
	if (flowingLiquid.size() > 0) {
		int fls = flowingLiquid.size();
		for (int i = 0; i < std::min(fls, 10); i++) {
			guVector liquid = flowingLiquid[0];
			flowingLiquid.erase(flowingLiquid.begin());
			guVector newLiquid;
			uint8_t liquidType = this->getBlock(liquid.x, liquid.y, liquid.z);
			uint8_t bottomBlock = this->getBlock(liquid.x, liquid.y-1, liquid.z);
			if (bottomBlock == 0 && liquidType != 0) {
				this->setBlock(liquid.x, liquid.y-1, liquid.z, liquidType);
				newLiquid.x = liquid.x;
				newLiquid.y = liquid.y-1;
				newLiquid.z = liquid.z;
				flowingLiquid.push_back(newLiquid);
			} else if (bottomBlock != liquidType) {
				trySetLiquidBlock(x-1,y,z);
				trySetLiquidBlock(x+1,y,z);
				trySetLiquidBlock(x,y,z-1);
				trySetLiquidBlock(x,y,z+1);
			}
		}
	}
}

#undef trySetLiquidBlock

void World::placeTree(int x, int y, int z) {
	if (this->getBlock(x,y-1,z) == 2)
		this->setBlockRaw(x,y-1,z,3);
	int lx,ly,lz;
	for (ly = y + 3; ly <= y + 4; ly++) {
		for (lx = x - 2; lx <= x + 2; lx++) {
			for (lz = z - 2; lz <= z + 2; lz++) {
				setIfAir(lx,ly,lz,18);
			}
		}
	}
	for (lx = x - 1; lx <= x + 1; lx++) {
		for (lz = z - 1; lz <= z + 1; lz++) {
			setIfAir(lx,y+5,lz,18);
		}
	}
	setIfAir(x,y+6,z,18);
	setIfAir(x-1,y+6,z,18);
	setIfAir(x+1,y+6,z,18);
	setIfAir(x,y+6,z-1,18);
	setIfAir(x,y+6,z+1,18);
	for (ly = y; ly < y + 6; ly++)
		this->setBlockRaw(x,ly,z,17);
}
