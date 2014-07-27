#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_brick.h"
#include "brick.h"

GRRLIB_texImg *tex_brick;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_brick);
}

void brick_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(45, entry);
	tex_brick = GRRLIB_LoadTexture(block_brick);
}

void brick_clean() {
	GRRLIB_FreeTexture(tex_brick);
}
