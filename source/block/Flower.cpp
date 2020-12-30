#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Flower.hpp"

static blockTexture *tex_flower;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlockCrossed(xPos, yPos, zPos, tex_flower);
}

void flower_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(37, entry);
	tex_flower = getTexture(13, 0);
}
