#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_sand.h"
#include "sand.h"

GRRLIB_texImg *tex_sand;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_sand);
}

void sand_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(12, entry);
	tex_sand = GRRLIB_LoadTexture(block_sand);
}

void sand_clean() {
	GRRLIB_FreeTexture(tex_sand);
}
