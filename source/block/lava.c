#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_lava.h"
#include "lava.h"

GRRLIB_texImg *tex_lava;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_lava, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void lava_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(10, entry);
	tex_lava = GRRLIB_LoadTexture(block_lava);
}

void lava_clean() {
	GRRLIB_FreeTexture(tex_lava);
}
