#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_white.h"
#include "cloth_white.h"

GRRLIB_texImg *tex_cloth_white;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_white, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_white_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(36, entry);
	tex_cloth_white = GRRLIB_LoadTexture(block_cloth_white);
}

void cloth_white_clean() {
	GRRLIB_FreeTexture(tex_cloth_white);
}
