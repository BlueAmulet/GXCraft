#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "mushroom.h"

blockTexture *tex_mushroom;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_mushroom);
}

void mushroom_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(39, entry);
	tex_mushroom = getTexture(13, 1);
}
