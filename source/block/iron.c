#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_iron_top.h"
#include "../textures/block_iron_bott.h"
#include "../textures/block_iron_side.h"
#include "iron.h"

GRRLIB_texImg *tex_iron_top;
GRRLIB_texImg *tex_iron_bott;
GRRLIB_texImg *tex_iron_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_iron_top);
}

void iron_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(42, entry);
	tex_iron_top = GRRLIB_LoadTexture(block_iron_top);
	tex_iron_bott = GRRLIB_LoadTexture(block_iron_bott);
	tex_iron_side = GRRLIB_LoadTexture(block_iron_side);
}

void iron_clean() {
	GRRLIB_FreeTexture(tex_iron_top);
	GRRLIB_FreeTexture(tex_iron_bott);
	GRRLIB_FreeTexture(tex_iron_side);
}
