#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "ore_coal.h"

blockTexture *tex_ore_coal;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_ore_coal);
}

void ore_coal_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(16, entry);
	tex_ore_coal = getTexture(2, 2);
}
