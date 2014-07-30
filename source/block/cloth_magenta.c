#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "cloth_magenta.h"

blockTexture *tex_cloth_magenta;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_cloth_magenta);
}

void cloth_magenta_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(32, entry);
	tex_cloth_magenta = getTexture(12, 4);
}
