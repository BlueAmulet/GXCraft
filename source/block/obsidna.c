#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_obsidna.h"
#include "obsidna.h"

GRRLIB_texImg *tex_obsidna;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_obsidna);
}

void obsidna_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(49, entry);
	tex_obsidna = GRRLIB_LoadTexture(block_obsidna);
}

void obsidna_clean() {
	GRRLIB_FreeTexture(tex_obsidna);
}
