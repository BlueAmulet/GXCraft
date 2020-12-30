#include <grrlib.h>

#include "../Block.hpp"
#include "../Render.hpp"

#include "ClothMagenta.hpp"

static blockTexture *tex_cloth_magenta;

static void render(s16 xPos, s16 yPos, s16 zPos, unsigned char pass) {
	if (pass == 1)
		return;
	Render::drawBlock(xPos, yPos, zPos, tex_cloth_magenta);
}

void cloth_magenta_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(32, entry);
	tex_cloth_magenta = getTexture(12, 4);
}
