#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_mushroom.h"
#include "mushroom.h"

GRRLIB_texImg *tex_mushroom;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_mushroom);
}

void mushroom_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(39, entry);
	tex_mushroom = GRRLIB_LoadTexture(block_mushroom);
}

void mushroom_clean() {
	GRRLIB_FreeTexture(tex_mushroom);
}
