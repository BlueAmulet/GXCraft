#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_stone.h"
#include "stone.h"

GRRLIB_texImg *tex_stone;

void render(unsigned char blockID, int xPos, int yPos, int zPos) {
	GRRLIB_SetTexture(tex_stone, 0);
	drawBlock(blockID, xPos, yPos, zPos);
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
