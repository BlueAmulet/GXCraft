#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_orange.h"
#include "cloth_orange.h"

GRRLIB_texImg *tex_cloth_orange;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_orange, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_orange_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(22, entry);
	tex_cloth_orange = GRRLIB_LoadTexture(block_cloth_orange);
}

void cloth_orange_clean() {
	GRRLIB_FreeTexture(tex_cloth_orange);
}
