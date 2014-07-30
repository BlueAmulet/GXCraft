#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_white.h"

blockTexture *tex_cloth_white;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_white);
}

void cloth_white_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(36, entry);
	tex_cloth_white = getTexture(15, 4);
}
