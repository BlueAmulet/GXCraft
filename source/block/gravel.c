#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_gravel.h"
#include "gravel.h"

GRRLIB_texImg *tex_gravel;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_gravel, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void gravel_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(13, entry);
	tex_gravel = GRRLIB_LoadTexture(block_gravel);
}

void gravel_clean() {
	GRRLIB_FreeTexture(tex_gravel);
}
