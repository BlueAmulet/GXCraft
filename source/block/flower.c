#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_flower.h"
#include "flower.h"

GRRLIB_texImg *tex_flower;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_flower);
}

void flower_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(37, entry);
	tex_flower = GRRLIB_LoadTexture(block_flower);
}

void flower_clean() {
	GRRLIB_FreeTexture(tex_flower);
}
