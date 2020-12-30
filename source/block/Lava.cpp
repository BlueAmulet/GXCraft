#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Lava.hpp"

static blockTexture *tex_lava;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 0)
		return;
	Render::drawLiquidBlock(xPos, yPos, zPos, tex_lava);
}

void lava_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(10, entry);
	registerBlock(11, entry);
	tex_lava = getTexture(14, 1);
}
