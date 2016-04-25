#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "sapling.h"

blockTexture *tex_sapling;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_sapling);
}

void sapling_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(6, entry);
	tex_sapling = getTexture(15, 0);
}
