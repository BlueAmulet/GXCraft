#pragma once

#include <cstdint>

#include <gccore.h>

typedef struct blockEntry_s {
	void (*renderBlock)(s16 xPos, s16 yPos, s16 zPos, unsigned char pass);
} blockEntry;

extern blockEntry blockRegistry[256];

extern void registerBlock(uint8_t id, blockEntry entry);
