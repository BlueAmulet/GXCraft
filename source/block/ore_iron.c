#include <grrlib.h>

#include "../block.h"
#include "../render.h"
#include "../textures/block_ore_iron.h"
#include "ore_iron.h"

GRRLIB_texImg *tex_ore_iron;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_ore_iron);
}

void ore_iron_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(15, entry);
	tex_ore_iron = GRRLIB_LoadTexture(block_ore_iron);
}

void ore_iron_clean() {
	GRRLIB_FreeTexture(tex_ore_iron);
}
