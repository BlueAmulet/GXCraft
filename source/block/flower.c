#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "flower.h"

blockTexture *tex_flower;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_flower);
}

void flower_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(37, entry);
	tex_flower = getTexture(13, 0);
}
