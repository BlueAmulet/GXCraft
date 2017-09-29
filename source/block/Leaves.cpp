#include <grrlib.h>

#include "../Main.hpp"
#include "../Block.hpp"
#include "../Render.hpp"
#include "../DisplayList.hpp"

#include "Leaves.hpp"

static blockTexture *tex_leaves;

inline void drawLeavesBlock(int xPos, int yPos, int zPos);

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	Render::drawBlock(xPos, yPos, zPos, tex_leaves);
}

void leaves_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(18, entry);
	tex_leaves = getTexture(6, 1);
}
