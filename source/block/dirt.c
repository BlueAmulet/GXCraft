#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_dirt.h"
#include "dirt.h"

GRRLIB_texImg *tex_dirt;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_dirt);
}

void dirt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(3, entry);
	tex_dirt = GRRLIB_LoadTexture(block_dirt);
}

void dirt_clean() {
	GRRLIB_FreeTexture(tex_dirt);
}
