#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_sponge.h"
#include "sponge.h"

GRRLIB_texImg *tex_sponge;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_sponge, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void sponge_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(19, entry);
	tex_sponge = GRRLIB_LoadTexture(block_sponge);
}

void sponge_clean() {
	GRRLIB_FreeTexture(tex_sponge);
}
