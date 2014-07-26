#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_redshroom.h"
#include "redshroom.h"

GRRLIB_texImg *tex_redshroom;

static void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	if (lastID != blockID)
		GRRLIB_SetTexture(tex_redshroom, 0);
	drawBlock(blockID, xPos, yPos, zPos);
}

void redshroom_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(40, entry);
	tex_redshroom = GRRLIB_LoadTexture(block_redshroom);
}

void redshroom_clean() {
	GRRLIB_FreeTexture(tex_redshroom);
}
