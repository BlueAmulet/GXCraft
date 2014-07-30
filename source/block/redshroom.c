#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "redshroom.h"

blockTexture *tex_redshroom;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	drawBlockCrossed(xPos, yPos, zPos, tex_redshroom);
}

void redshroom_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(40, entry);
	tex_redshroom = getTexture(12, 1);
}
