#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_green.h"
#include "cloth_green.h"

GRRLIB_texImg *tex_cloth_green;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_cloth_green, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void cloth_green_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(25, entry);
	tex_cloth_green = GRRLIB_LoadTexture(block_cloth_green);
}

void cloth_green_clean() {
	GRRLIB_FreeTexture(tex_cloth_green);
}
