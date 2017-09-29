#include <grrlib.h>

#include "../Main.hpp"
#include "../Block.hpp"
#include "../Render.hpp"
#include "../DisplayList.hpp"

#include "Water.hpp"

static blockTexture *tex_water;

inline void drawWaterBlock(int xPos, int yPos, int zPos);

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 0) return;
	Render::drawLiquidBlock(xPos, yPos, zPos, tex_water);
}

void water_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(8, entry);
	registerBlock(9, entry);
	tex_water = getTexture(14, 0);
}
