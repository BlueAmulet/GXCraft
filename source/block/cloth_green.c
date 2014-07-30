#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_green.h"

blockTexture *tex_cloth_green;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_green);
}

void cloth_green_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(25, entry);
	tex_cloth_green = getTexture(4, 4);
}
