#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "brick.h"

blockTexture *tex_brick;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_brick);
}

void brick_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(45, entry);
	tex_brick = getTexture(7, 0);
}
