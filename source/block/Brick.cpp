#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Brick.hpp"

static blockTexture *tex_brick;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_brick);
}

void brick_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(45, entry);
	tex_brick = getTexture(7, 0);
}
