#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "OreIron.hpp"

static blockTexture *tex_ore_iron;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_ore_iron);
}

void ore_iron_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(15, entry);
	tex_ore_iron = getTexture(1, 2);
}
