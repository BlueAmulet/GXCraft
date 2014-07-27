#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_mossy.h"
#include "mossy.h"

GRRLIB_texImg *tex_mossy;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_mossy);
}

void mossy_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(48, entry);
	tex_mossy = GRRLIB_LoadTexture(block_mossy);
}

void mossy_clean() {
	GRRLIB_FreeTexture(tex_mossy);
}
