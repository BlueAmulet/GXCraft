#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Sapling.hpp"

static blockTexture *tex_sapling;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlockCrossed(xPos, yPos, zPos, tex_sapling);
}

void sapling_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(6, entry);
	tex_sapling = getTexture(15, 0);
}
