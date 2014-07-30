#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "ore_gold.h"

blockTexture *tex_ore_gold;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_ore_gold);
}

void ore_gold_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(14, entry);
	tex_ore_gold = getTexture(0, 2);
}
