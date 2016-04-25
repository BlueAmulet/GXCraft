#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "redshroom.hpp"

static blockTexture *tex_redshroom;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlockCrossed(xPos, yPos, zPos, tex_redshroom);
}

void redshroom_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(40, entry);
	tex_redshroom = getTexture(12, 1);
}
