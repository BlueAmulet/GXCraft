#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "gravel.h"

blockTexture *tex_gravel;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_gravel);
}

void gravel_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(13, entry);
	tex_gravel = getTexture(3, 1);
}
