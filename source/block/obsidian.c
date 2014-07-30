#include <grrlib.h>

#include "../block.h"
#include "../render.h"

#include "obsidian.h"

blockTexture *tex_obsidian;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_obsidian);
}

void obsidian_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(49, entry);
	tex_obsidian = getTexture(5, 2);
}
