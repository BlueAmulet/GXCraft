#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_pink.h"

blockTexture *tex_cloth_pink;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_pink);
}

void cloth_pink_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(33, entry);
	tex_cloth_pink = getTexture(11, 4);
}
