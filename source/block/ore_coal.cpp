#include <grrlib.h>

#include "../block.hpp"
#include "../render.hpp"

#include "ore_coal.hpp"

static blockTexture *tex_ore_coal;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_ore_coal);
}

void ore_coal_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(16, entry);
	tex_ore_coal = getTexture(2, 2);
}
