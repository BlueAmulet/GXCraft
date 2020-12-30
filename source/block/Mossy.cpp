#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Mossy.hpp"

static blockTexture *tex_mossy;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_mossy);
}

void mossy_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(48, entry);
	tex_mossy = getTexture(4, 2);
}
