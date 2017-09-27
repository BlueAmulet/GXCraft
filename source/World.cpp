#include <cstdlib>
#include <cmath>
#include <cstdint>

#include "FastNoise.hpp"

#include "Main.hpp"
#include "World.hpp"
#include "ChunkedRender.hpp"

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

World::World(unsigned int seed) {
	FastNoise worldNoise(seed);
	worldNoise.SetFrequency(1.0/256.0);
	worldNoise.SetFractalOctaves(5);

	flowingLiquid.reserve(32);
	memset(theWorld, 255, sizeof theWorld);

	short x,y,z;
	for (x = 0; x < worldX; x++) {
		for (z = 0; z < worldZ; z++) {
			int terrainPiece = (worldNoise.GetSimplexFractal(x, z) / 2.0 + 0.5) * worldY;
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
