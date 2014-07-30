#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_yellow.h"

blockTexture *tex_cloth_yellow;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_yellow);
}

void cloth_yellow_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(23, entry);
	tex_cloth_yellow = getTexture(2, 4);
}
