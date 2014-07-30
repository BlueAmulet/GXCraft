#include <grrlib.h>

#include "../block.h"
#include "../render.h"


#include "slab.h"

blockTexture *tex_slab_top;
blockTexture *tex_slab_side;

static void render(int xPos, int yPos, int zPos, unsigned char pass) {
	if (pass == 1) return;
	drawBlock(xPos, yPos, zPos, tex_slab_top);
}

void slab_init() {
	blockEntry entry;
	entry.renderBlock = render;
	registerBlock(43, entry);
	tex_slab_top = getTexture(6, 0);
	tex_slab_side = getTexture(5, 0);
}
