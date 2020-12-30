#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Iron.hpp"

static blockTexture *tex_iron_top;
static blockTexture *tex_iron_bott;
static blockTexture *tex_iron_side;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawMultiTexBlock(xPos, yPos, zPos, tex_iron_top, tex_iron_side, tex_iron_bott);
}

void iron_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(42, entry);
	tex_iron_top = getTexture(7, 1);
	tex_iron_bott = getTexture(7, 3);
	tex_iron_side = getTexture(7, 2);
}
