#pragma once
#include "display_list.hpp"
#include "player.hpp"

#define nRenderChunks 256

typedef struct {
	bool active;
	bool update;
	unsigned short x, z;
	DisplayList *list;
	DisplayList *blendlist;
} renderchunk;

class Chunked {
	private:
		static renderchunk *renderchunks[nRenderChunks];

		static void deallocall();
		static int getchunkfromchunkpos(short unsigned int, short unsigned int);
		static int chunk_cmp(const void*, const void*);
	public:
		static void init();
		static void markchunkforupdate(unsigned short, unsigned short);
		static void rerenderChunkUpdates();
		static void rerenderChunk(signed short, signed short, bool);
		static void refresh(int, player);
		static void render(player);
		static int getfifousage();
		static int getfifototal();
};
