#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_stone.h"
#include "stone.h"

GRRLIB_texImg *tex_stone;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_stone);
}

void stone_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(1, entry);
	tex_stone = GRRLIB_LoadTexture(block_stone);
}

void stone_clean() {
	GRRLIB_FreeTexture(tex_stone);
}
