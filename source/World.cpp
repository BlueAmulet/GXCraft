#include <cstdlib>
#include <cmath>
#include <cstdint>

#include "FastNoiseLite.hpp"

#include "Main.hpp"
#include "World.hpp"
#include "ChunkedRender.hpp"

static int randnum(int x, int y) {
	double uni = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	return floor(uni * (y - x + 1)) + x;
}

static void updateNeighbors(int x, int z) {
	if (x % chunkSize == chunkSize-1) Chunked::markchunkforupdate(floor(x/chunkSize)+1,floor(z/chunkSize));
	if (x % chunkSize == 0)           Chunked::markchunkforupdate(floor(x/chunkSize)-1,floor(z/chunkSize));
	if (z % chunkSize == chunkSize-1) Chunked::markchunkforupdate(floor(x/chunkSize),floor(z/chunkSize)+1);
	if (z % chunkSize == 0)           Chunked::markchunkforupdate(floor(x/chunkSize),floor(z/chunkSize)-1);
}

static FastNoiseLite *createOctaveNoise(unsigned int seed, int octave, FastNoiseLite::NoiseType noiseType) {
	FastNoiseLite *noise = new FastNoiseLite(seed);
	noise->SetNoiseType(noiseType);
	noise->SetFractalType(FastNoiseLite::FractalType_FBm);
	noise->SetFractalLacunarity(0.5f);
	noise->SetFractalGain(2.0f);
	noise->SetFractalOctaves(octave);
	noise->SetFrequency(1.0f);
	return noise;
}

class CombinedNoise {
	private:
		FastNoiseLite *noise1;
		FastNoiseLite *noise2;
		float antiBound2;
	public:
		CombinedNoise(unsigned int seed1, unsigned int seed2, int octave1, int octave2, FastNoiseLite::NoiseType noiseType) {
			noise1 = createOctaveNoise(seed1, octave1, noiseType);
			noise2 = createOctaveNoise(seed2, octave2, noiseType);
			antiBound2 = pow(2.0f, static_cast<float>(octave2))-1.0f;
		}

		float GetNoise(float x, float y) {
			float offset = noise2->GetNoise(x, y) * antiBound2;
			return noise1->GetNoise(x + offset, y);
		}
};

static unsigned int _seed = 0xAB08E51C;

static unsigned int xorrand() {
	_seed ^= _seed << 13;
	_seed ^= _seed >> 17;
	_seed ^= _seed << 5;
	return _seed;
}

World::World(unsigned int seed) {
	_seed = (seed == 0 ? 0xAB08E51C : seed);
	CombinedNoise noise1(xorrand(), xorrand(), 8, 8, FastNoiseLite::NoiseType_Perlin); // Low Terrain
	CombinedNoise noise2(xorrand(), xorrand(), 8, 8, FastNoiseLite::NoiseType_Perlin); // High Terrain
	FastNoiseLite noise3 = *createOctaveNoise(xorrand(), 6, FastNoiseLite::NoiseType_Perlin); // Low/High select
	FastNoiseLite noise4 = *createOctaveNoise(xorrand(), 8, FastNoiseLite::NoiseType_Perlin); // Dirt Height
	FastNoiseLite noise5 = *createOctaveNoise(xorrand(), 8, FastNoiseLite::NoiseType_Perlin); // Dirt/Gravel select
	FastNoiseLite noise6 = *createOctaveNoise(xorrand(), 8, FastNoiseLite::NoiseType_Perlin); // Sand/Grass select
	FastNoiseLite noise7 = *createOctaveNoise(xorrand(), 1, FastNoiseLite::NoiseType_Value); // Trees
	FastNoiseLite noise8 = *createOctaveNoise(xorrand(), 8, FastNoiseLite::NoiseType_OpenSimplex2); // Flowers

	flowingLiquid.reserve(32);
	memset(theWorld, 255, sizeof theWorld);

	int waterLevel = worldY / 2;

	for (int x = 0; x < worldX; x++) {
		for (int z = 0; z < worldZ; z++) {
			float hLow = noise1.GetNoise(x * 1.3f, z * 1.3f) * 255 / 6 - 4;
			float height = hLow;

			if (noise3.GetNoise(static_cast<float>(x), static_cast<float>(z)) <= 0) {
				float hHigh = noise2.GetNoise(x * 1.3f, z * 1.3f) * 255 / 5 + 6;
				height = std::max(hLow, hHigh);
			}

			height *= 0.5f;
			if (height < 0)
				height *= 0.8f;

			int dirtHeight = height + waterLevel;
			int dirtThickness = noise4.GetNoise(static_cast<float>(x), static_cast<float>(z)) * 255 / 24 - 4;
			int stoneHeight = std::min(dirtHeight + dirtThickness, dirtHeight - 1);

			for (int y = 0; y < worldY; y++) {
				if (y == 0)
					theWorld[y][x][z] = 7;
				else if (y <= stoneHeight)
					theWorld[y][x][z] = 1;
				else if (y < dirtHeight)
					theWorld[y][x][z] = 3;
				else if (y == dirtHeight) {
					if (dirtHeight < (waterLevel - 1))
						theWorld[y][x][z] = ((noise5.GetNoise(static_cast<float>(x), static_cast<float>(z)) * 255) > 12) ? 13 : 3;
					else
						theWorld[y][x][z] = (y <= waterLevel && (noise6.GetNoise(static_cast<float>(x), static_cast<float>(z)) * 255) > 8) ? 12 : 2;
				} else if (y == dirtHeight + 1 && dirtHeight >= 32) {
					int place = floor((noise7.GetNoise(static_cast<float>(x), static_cast<float>(z)) / 2 + 0.5) * 512);
					if (place == 0)
						placeTree(x, y, z);
					if (place < 128) {
						float redPlace = noise8.GetNoise(x/64.0f, z/64.0f);
						if (redPlace > 0.8)
							theWorld[y][x][z] = 37;
						else if (redPlace < -0.8)
							theWorld[y][x][z] = 38;
					}
				}
				if (theWorld[y][x][z] == 255) {
					if (y < waterLevel)
						theWorld[y][x][z] = 8;
					else
						theWorld[y][x][z] = 0;
				}

				if (theWorld[y][x][z] != 0 && theWorld[y][x][z] != 6 && theWorld[y][x][z] != 18 && theWorld[y][x][z] != 20 && theWorld[y][x][z] != 37 && theWorld[y][x][z] != 38 && theWorld[y][x][z] != 39 && theWorld[y][x][z] != 40) {
					lighting[x][z] = y;
				}
			}
		}
	}
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
