#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_glass.h"
#include "glass.h"

GRRLIB_texImg *tex_glass;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_glass, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void glass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(20, entry);
	tex_glass = GRRLIB_LoadTexture(block_glass);
}

void glass_clean() {
	GRRLIB_FreeTexture(tex_glass);
}
