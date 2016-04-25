#pragma once
#include "display_list.hpp"
#include "block_textures.hpp"

class Render {
	private:
		static bool testFace(u8);
	public:
		static void bind(DisplayList*);
		static DisplayList* getbound();

		static void drawBlock(int, int, int, blockTexture*);
		static void drawBlockCrossed(int, int, int, blockTexture*);
		static void drawMultiTexBlock(int, int, int, blockTexture*, blockTexture*, blockTexture*);
		static void drawSelectionBlock(int, int, int);

		static void drawText(f32, f32, GRRLIB_texImg*, const char*, ...);
};
