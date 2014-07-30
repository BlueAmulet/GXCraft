#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_gray.h"

blockTexture *tex_cloth_gray;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_gray);
}

void cloth_gray_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(35, entry);
	tex_cloth_gray = getTexture(14, 4);
}
