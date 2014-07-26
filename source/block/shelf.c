#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_shelf.h"
#include "shelf.h"

GRRLIB_texImg *tex_shelf;

static void render(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_shelf, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void shelf_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(47, entry);
	tex_shelf = GRRLIB_LoadTexture(block_shelf);
}

void shelf_clean() {
	GRRLIB_FreeTexture(tex_shelf);
}
