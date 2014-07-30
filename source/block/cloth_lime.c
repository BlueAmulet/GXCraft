#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_lime.h"

blockTexture *tex_cloth_lime;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_lime);
}

void cloth_lime_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(24, entry);
	tex_cloth_lime = getTexture(3, 4);
}
