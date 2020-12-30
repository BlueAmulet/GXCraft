#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Tnt.hpp"

static blockTexture *tex_tnt_top;
static blockTexture *tex_tnt_bott;
static blockTexture *tex_tnt_side;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_tnt_top, tex_tnt_side, tex_tnt_bott);
}

void tnt_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(46, entry);
	tex_tnt_top = getTexture(9, 0);
	tex_tnt_bott = getTexture(10, 0);
	tex_tnt_side = getTexture(8, 0);
}
