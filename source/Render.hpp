#pragma once
#include "DisplayList.hpp"
#include "BlockTextures.hpp"

namespace Render {
	void bind(DisplayList*);
	DisplayList* getbound();

	void drawText(f32, f32, GRRLIB_texImg*, const char*, ...);

	void drawBlock(int, int, int, blockTexture*);
	void drawBlockCrossed(int, int, int, blockTexture*);
	void drawMultiTexBlock(int, int, int, blockTexture*, blockTexture*, blockTexture*);
	void drawSelectionBlock(int, int, int);
};
