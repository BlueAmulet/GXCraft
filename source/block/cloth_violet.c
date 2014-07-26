#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_violet.h"
#include "cloth_violet.h"

GRRLIB_texImg *tex_cloth_violet;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_violet, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_violet_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(31, entry);
	tex_cloth_violet = GRRLIB_LoadTexture(block_cloth_violet);
}

void cloth_violet_clean() {
	GRRLIB_FreeTexture(tex_cloth_violet);
}
