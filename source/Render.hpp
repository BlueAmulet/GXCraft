#pragma once

#include <gctypes.h>
#include <grrlib.h>

#include "DisplayList.hpp"
#include "BlockTextures.hpp"

namespace Render {
	void bind(DisplayList*);

	void drawText(f32, f32, GRRLIB_texImg*, const char*, ...);

	void drawBlock(s16, s16, s16, blockTexture*);
	void drawBlockCrossed(s16, s16, s16, blockTexture*);
	void drawMultiTexBlock(s16, s16, s16, blockTexture*, blockTexture*, blockTexture*);
	void drawLiquidBlock(s16, s16, s16, blockTexture*);
	void drawSelectionBlock(f32, f32, f32);
}
