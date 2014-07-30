#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cobble.h"

blockTexture *tex_cobble;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cobble);
}

void cobble_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(4, entry);
	tex_cobble = getTexture(0, 1);
}
