#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "shelf.h"

blockTexture *tex_shelf;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_shelf);
}

void shelf_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(47, entry);
	tex_shelf = getTexture(3, 2);
}
