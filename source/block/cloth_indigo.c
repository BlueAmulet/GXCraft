#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_cloth_indigo.h"
#include "cloth_indigo.h"

GRRLIB_texImg *tex_cloth_indigo;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_indigo);
}

void cloth_indigo_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(30, entry);
	tex_cloth_indigo = GRRLIB_LoadTexture(block_cloth_indigo);
}

void cloth_indigo_clean() {
	GRRLIB_FreeTexture(tex_cloth_indigo);
}
