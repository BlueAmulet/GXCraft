#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Mushroom.hpp"

static blockTexture *tex_mushroom;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlockCrossed(xPos, yPos, zPos, tex_mushroom);
}

void mushroom_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(39, entry);
	tex_mushroom = getTexture(13, 1);
}
