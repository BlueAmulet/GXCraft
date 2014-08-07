#include <grrlib.h>

#include "../block.h"
#include "../render.h"



#include "tnt.h"

blockTexture *tex_tnt_top;
blockTexture *tex_tnt_bott;
blockTexture *tex_tnt_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawMultiTexBlock(xPos, yPos, zPos, tex_tnt_top, tex_tnt_side, tex_tnt_bott);
}

void tnt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(46, entry);
	tex_tnt_top = getTexture(9, 0);
	tex_tnt_bott = getTexture(10, 0);
	tex_tnt_side = getTexture(8, 0);
}
