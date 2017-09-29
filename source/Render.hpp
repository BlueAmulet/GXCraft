#pragma once

#include <gctypes.h>
#include <grrlib.h>

#include "DisplayList.hpp"
#include "BlockTextures.hpp"

namespace Render {
	void bind(DisplayList*);

	void drawText(f32, f32, GRRLIB_texImg*, const char*, ...);

	void drawBlock(int, int, int, blockTexture*);
	void drawBlockCrossed(int, int, int, blockTexture*);
	void drawMultiTexBlock(int, int, int, blockTexture*, blockTexture*, blockTexture*);
	void drawLiquidBlock(int, int, int, blockTexture*);
	void drawSelectionBlock(int, int, int);
};
