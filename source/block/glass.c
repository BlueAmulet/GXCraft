#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "glass.h"

blockTexture *tex_glass;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlock(xPos, yPos, zPos, tex_glass);
}

void glass_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(20, entry);
	tex_glass = getTexture(1, 3);
}
