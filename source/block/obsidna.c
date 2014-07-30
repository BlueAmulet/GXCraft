#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "obsidna.h"

blockTexture *tex_obsidna;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_obsidna);
}

void obsidna_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(49, entry);
	tex_obsidna = getTexture(5, 2);
}
