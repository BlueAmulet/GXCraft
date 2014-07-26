#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_dirt.h"
#include "dirt.h"

GRRLIB_texImg *tex_dirt;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_dirt, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void dirt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(3, entry);
	tex_dirt = GRRLIB_LoadTexture(block_dirt);
}

void dirt_clean() {
	GRRLIB_FreeTexture(tex_dirt);
}
