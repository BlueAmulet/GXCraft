#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_aqua.h"

blockTexture *tex_cloth_aqua;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_aqua);
}

void cloth_aqua_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(26, entry);
	tex_cloth_aqua = getTexture(5, 4);
}
