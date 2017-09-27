#pragma once

#include "DisplayList.hpp"

#define nRenderChunks 256

struct RenderChunk {
	bool active;
	bool update;
	unsigned short x, z;
	DisplayList *list;
	DisplayList *blendlist;
};

namespace Chunked {
	void init();
	void deallocall();
	void markchunkforupdate(unsigned short, unsigned short);
	bool rerenderChunkUpdates(bool);
	void rerenderChunk(signed short, signed short, bool);
	void refresh(int);
	void render();
	void getfifousage(int*, int*);
};
