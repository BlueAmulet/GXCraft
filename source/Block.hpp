#pragma once

#include <cstdint>

typedef struct blockEntry_s {
	void (*renderBlock)(int xPos, int yPos, int zPos, unsigned char pass);
} blockEntry;

extern blockEntry blockRegistry[256];

extern void registerBlock(uint8_t id, blockEntry entry);
