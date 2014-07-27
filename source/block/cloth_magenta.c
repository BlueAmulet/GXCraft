#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_magenta.h"
#include "cloth_magenta.h"

GRRLIB_texImg *tex_cloth_magenta;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_magenta);
}

void cloth_magenta_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(32, entry);
	tex_cloth_magenta = GRRLIB_LoadTexture(block_cloth_magenta);
}

void cloth_magenta_clean() {
	GRRLIB_FreeTexture(tex_cloth_magenta);
}
