#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothCyan.hpp"

static blockTexture *tex_cloth_cyan;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_cyan);
}

void cloth_cyan_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(27, entry);
	tex_cloth_cyan = getTexture(6, 4);
}
