#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "sand.h"

blockTexture *tex_sand;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_sand);
}

void sand_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(12, entry);
	tex_sand = getTexture(2, 1);
}
