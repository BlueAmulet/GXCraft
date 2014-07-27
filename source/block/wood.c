#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_wood.h"
#include "wood.h"

GRRLIB_texImg *tex_wood;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_wood);
}

void wood_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(5, entry);
	tex_wood = GRRLIB_LoadTexture(block_wood);
}

void wood_clean() {
	GRRLIB_FreeTexture(tex_wood);
}
