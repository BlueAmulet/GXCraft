#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_black.h"

blockTexture *tex_cloth_black;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_black);
}

void cloth_black_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(34, entry);
	tex_cloth_black = getTexture(13, 4);
}
