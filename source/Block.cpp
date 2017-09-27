#include <cstdint>

#include "Block.hpp"

blockEntry blockRegistry[256];

void registerBlock(uint8_t id, blockEntry entry) {
	blockRegistry[id] = entry;
}
