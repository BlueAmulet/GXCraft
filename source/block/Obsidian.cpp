#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "Obsidian.hpp"

static blockTexture *tex_obsidian;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_obsidian);
}

void obsidian_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(49, entry);
	tex_obsidian = getTexture(5, 2);
}
