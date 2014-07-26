#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_leaves.h"
#include "leaves.h"

GRRLIB_texImg *tex_leaves;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_leaves, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void leaves_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(18, entry);
	tex_leaves = GRRLIB_LoadTexture(block_leaves);
}

void leaves_clean() {
	GRRLIB_FreeTexture(tex_leaves);
}
