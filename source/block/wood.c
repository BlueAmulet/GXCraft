#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "wood.h"

blockTexture *tex_wood;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_wood);
}

void wood_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(5, entry);
	tex_wood = getTexture(4, 0);
}
