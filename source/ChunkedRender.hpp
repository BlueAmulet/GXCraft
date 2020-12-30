#pragma once

#include "DisplayList.hpp"

#define nRenderChunks 256

struct RenderChunk {
	short x, z;
	DisplayList *list;
	DisplayList *blendlist;
	bool active;
	bool update;
};

namespace Chunked {
	void init();
	void deallocall();
	void markchunkforupdate(short, short);
	bool rerenderChunkUpdates(bool);
	void rerenderChunk(short, short, bool);
	void refresh(int);
	void render();
	void getfifousage(size_t*, size_t*);
}
