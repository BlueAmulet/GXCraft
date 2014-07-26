#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_sapling.h"
#include "sapling.h"

GRRLIB_texImg *tex_sapling;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_sapling, 0);
	drawBlockCrossed(blockID, xPos, yPos, zPos);
}

void sapling_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(6, entry);
	tex_sapling = GRRLIB_LoadTexture(block_sapling);
}

void sapling_clean() {
	GRRLIB_FreeTexture(tex_sapling);
}
