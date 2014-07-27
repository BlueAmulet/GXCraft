#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_sponge.h"
#include "sponge.h"

GRRLIB_texImg *tex_sponge;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_sponge);
}

void sponge_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(19, entry);
	tex_sponge = GRRLIB_LoadTexture(block_sponge);
}

void sponge_clean() {
	GRRLIB_FreeTexture(tex_sponge);
}
