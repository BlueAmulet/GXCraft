#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "lava.h"

blockTexture *tex_lava;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_lava);
}

void lava_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(10, entry);
	tex_lava = getTexture(14, 1);
}
