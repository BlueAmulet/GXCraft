#include <gctypes.h>
#include "block.hpp"

blockEntry blockRegistry[256];

void registerBlock(u8 id, blockEntry entry) {
	blockRegistry[id] = entry;
}
