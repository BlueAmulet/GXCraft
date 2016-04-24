#include <gctypes.h>
#include "block.h"

blockEntry blockRegistry[256];

void registerBlock(u8 id, blockEntry entry) {
	blockRegistry[id] = entry;
}
