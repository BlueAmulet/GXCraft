#include "block.h"

blockEntry blockRegistry[256];

void registerBlock(unsigned char id, blockEntry entry) {
	blockRegistry[id] = entry;
}
