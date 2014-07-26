#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_blue.h"
#include "cloth_blue.h"

GRRLIB_texImg *tex_cloth_blue;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_blue, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_blue_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(28, entry);
	tex_cloth_blue = GRRLIB_LoadTexture(block_cloth_blue);
}

void cloth_blue_clean() {
	GRRLIB_FreeTexture(tex_cloth_blue);
}
