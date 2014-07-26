#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cobble.h"
#include "cobble.h"

GRRLIB_texImg *tex_cobble;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cobble, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cobble_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(4, entry);
	tex_cobble = GRRLIB_LoadTexture(block_cobble);
}

void cobble_clean() {
	GRRLIB_FreeTexture(tex_cobble);
}
