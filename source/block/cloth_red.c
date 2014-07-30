#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_red.h"

blockTexture *tex_cloth_red;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_red);
}

void cloth_red_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(21, entry);
	tex_cloth_red = getTexture(0, 4);
}
