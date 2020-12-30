#pragma once

#include <gctypes.h>

class DisplayList {
private:
	s16 *vertex;
	u16 *color;
	u8 *texcoord;

public:
	u16 size;
	u16 index;

	static void start();

	DisplayList(u16);
	void clear();
	void add(s16, s16, s16, u16, f32, f32);
	void fit();
	void render();
};
