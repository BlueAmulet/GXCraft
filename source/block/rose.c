#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_rose.h"
#include "rose.h"

GRRLIB_texImg *tex_rose;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_rose);
}

void rose_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(38, entry);
	tex_rose = GRRLIB_LoadTexture(block_rose);
}

void rose_clean() {
	GRRLIB_FreeTexture(tex_rose);
}
