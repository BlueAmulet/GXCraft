#include <grrlib.h>

#include "../block.h"
#include "../render.h"



#include "iron.h"

blockTexture *tex_iron_top;
blockTexture *tex_iron_bott;
blockTexture *tex_iron_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawMultiTexBlock(xPos, yPos, zPos, tex_iron_top, tex_iron_side, tex_iron_bott);
}

void iron_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(42, entry);
	tex_iron_top = getTexture(7, 1);
	tex_iron_bott = getTexture(7, 3);
	tex_iron_side = getTexture(7, 2);
}
