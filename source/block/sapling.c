#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_sapling.h"
#include "sapling.h"

GRRLIB_texImg *tex_sapling;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_sapling);
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
