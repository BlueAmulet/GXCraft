#include <cmath>
#include <cstdlib>
#include <algorithm>

#include <grrlib.h>

#include "ChunkedRender.hpp"
#include "NetcatLogger.hpp"
#include "Block.hpp"
#include "Render.hpp"
#include "Fail3D.hpp"
#include "Main.hpp"

#define chunked_isoob(rc,rcd,px,pz) (rc->x > px+rcd) || (rc->x < px-rcd) || (rc->z > pz+rcd) || (rc->z < pz-rcd)

static RenderChunk* renderchunks[nRenderChunks];
static int renderorder[nRenderChunks];

static int getchunkfromchunkpos(unsigned short x, unsigned short z) {
	//first try to find an existing renderchunk with the coords
	for (int i=0; i<nRenderChunks; i++) {
		RenderChunk *rc = renderchunks[i];
		if (rc->active && rc->x == x && rc->z == z) {
			Netcat::log("found active chunk with same x and z\n");
			return i;
		}
	}
	//else, find an inactive chunk
	for (int i=0; i<nRenderChunks; i++) {
		RenderChunk *rc = renderchunks[i];
		if (!rc->active) {
			Netcat::log("found inactive chunk\n");
			rc->x = x;
			rc->z = z;
			return i;
		}
	}
	Netcat::log("no more chunks!\n");
	return -1;
}

static Player cmp_player;
static int chunk_cmp(const void *a, const void *b) {
	RenderChunk *ca = renderchunks[*((const int *)a)];
	RenderChunk *cb = renderchunks[*((const int *)b)];
	int adst = abs(ca->x*chunkSize - cmp_player.posX) + abs(ca->z*chunkSize - cmp_player.posZ);
	int bdst = abs(cb->x*chunkSize - cmp_player.posX) + abs(cb->z*chunkSize - cmp_player.posZ);
	if (adst > bdst)
		return -1;
	else if (adst < bdst)
		return 1;
	else
		return 0;
}

static void calculateChunkPoint(guVector *polygon, guVector *center, guVector *camera) {
	Fail3D::translatePoint(polygon, center);
	Fail3D::rotatePoint(polygon, camera);
	Fail3D::calculatePointPosition(polygon);
}

static inline void renderNewChunk(signed short cx, signed short cz) {
	if (cx < 0 || cz < 0 || cx >= worldX/chunkSize || cz >= worldZ/chunkSize)
		return;
	RenderChunk *rc = renderchunks[getchunkfromchunkpos(cx,cz)];
	if (!rc->active) {
		rc->active = true;
		rc->update = true;
	}
}

namespace Chunked {
	void init() {
		//initialize all the renderchunks
		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = new RenderChunk();
			rc->active = false;
			rc->update = false;
			rc->list = NULL;
			rc->blendlist = NULL;
			renderchunks[i] = rc;
		}
	}

	void deallocall() {
		//dealloc all the renderchunks
		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = renderchunks[i];
			delete rc->list;
			rc->list = NULL;
			delete rc->blendlist;
			rc->blendlist = NULL;
			delete rc;
		}
	}

	void markchunkforupdate(unsigned short x, unsigned short z) {
		//try to find an existing renderchunk with the coords
		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = renderchunks[i];
			if (rc->active && rc->x == x && rc->z == z) {
				Netcat::log("marking active chunk for render update\n");
				rc->update = true;
				return;
			}
		}
		Netcat::log("no chunk to mark for render update!\n");
	}

	bool rerenderChunkUpdates(bool all) {
		bool rendered = false;
		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = renderchunks[i];
			if (rc->active && rc->update) {
				rerenderChunk(rc->x, rc->z, true);
				if (!all) // Slow chunk updates to avoid stutter
					return true;
				else
					rendered = true;
			}
		}
		return rendered;
	}

	inline void rerenderChunk(signed short cx, signed short cz, bool force) {
		if (cx < 0 || cz < 0 || cx >= worldX/chunkSize || cz >= worldZ/chunkSize)
			return;
		RenderChunk *rc = renderchunks[getchunkfromchunkpos(cx,cz)];
		if ((!rc->active) || rc->update || force) {
			rc->active = true;
			rc->update = false;
			Netcat::logf("rendering chunk %d, %d\n",cx,cz);
			//check for display list
			if (rc->list == NULL) {
				rc->list = new DisplayList(256);
				rc->blendlist = new DisplayList(256);
			}
			//start rendering blocks
			int bx, bz;
			bx = cx*chunkSize;
			bz = cz*chunkSize;

			rc->list->clear();
			Render::bind(rc->list);
			int x;
			int y;
			int z;
			for (y = worldY - 1; y >= 0; y--) {
				for (x = bx; x < bx+chunkSize; x++) {
					for (z = bz; z < bz+chunkSize; z++) {
						uint8_t blockID = theWorld->theWorld[y][x][z];
						if (blockID != 0) {
							blockEntry entry = blockRegistry[blockID];
							if (entry.renderBlock != NULL)
								entry.renderBlock(x, y, z, 0);
						}
					}
				}
			}
			rc->list->fit();
			Netcat::log("rendered pass 0\n");

			rc->blendlist->clear();
			Render::bind(rc->blendlist);
			for (y = 0; y < worldY; y++) {
				for (x = bx; x < bx+chunkSize; x++) {
					for (z = bz; z < bz+chunkSize; z++) {
						uint8_t blockID = theWorld->theWorld[y][x][z];
						if (blockID != 0) {
							blockEntry entry = blockRegistry[blockID];
							if (entry.renderBlock != NULL)
								entry.renderBlock(x, y, z, 1);
						}
					}
				}
			}
			rc->blendlist->fit();
			Netcat::log("rendered pass 1\n");
			Netcat::logf("%d/%d\n",rc->list->index,rc->list->size);
		}
		else
		{
			Netcat::logf("chunk %d, %d already rendered\n",cx,cz);
		}
	}

	void refresh(int renderDistance) {
		//convert the player's position to chunk position
		signed short px, pz;
		px = thePlayer.posX/chunkSize;
		pz = thePlayer.posZ/chunkSize;
		int rcd = renderDistance/chunkSize+1;
		//remove the old chunks that are now out of range
		int nactive = 0;
		int nremoved = 0;
		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = renderchunks[i];
			if (rc->active) {
				if (chunked_isoob(rc, rcd, px, pz)) {
					rc->active = false;
					rc->update = false;
					nremoved++;
				}
				else
				{
					nactive++;
				}
			}
		}
		//if (nactive>0 && nremoved == 0) return; //could be problematic, attempts to stop if no chunks got removed
		//start rendering chunks
		unsigned short cx, cz;
		int maxcx, maxcz;
		maxcx = worldX/chunkSize;
		maxcz = worldZ/chunkSize;

		for (cx = std::max(px-rcd,0); cx < std::min(maxcx,px + rcd); cx++) {
			for (cz = std::max(pz-rcd,0); cz < std::min(maxcz,pz + rcd); cz++) {
				renderNewChunk(cx,cz);
			}
		}
	}

	void render() {
		guVector center = {thePlayer.posX, -thePlayer.posY - 1.625f, -thePlayer.posZ};
		guVector camera = {thePlayer.lookY, thePlayer.lookX, thePlayer.lookZ};
		guVector polygon;

		DisplayList::start();
		int nrendered = 0;
		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = renderchunks[i];
			if (rc->active && rc->list) {
				bool allL, allR, allU, allD;
				allL = allR = allU = allD = true;
				bool vZ = false;

#define testChunk(xval, yval, zval) \
polygon.x = xval; \
polygon.y = -(yval); \
polygon.z = -(zval); \
calculateChunkPoint(&polygon, &center, &camera); \
if (polygon.x >= 0) allL = false; \
if (polygon.x <= rmode->fbWidth) allR = false; \
if (polygon.y >= 0) allU = false; \
if (polygon.y <= rmode->efbHeight) allD = false; \
if (polygon.z > 0) vZ = true;
				testChunk(rc->x*chunkSize,           0,      rc->z*chunkSize);
				testChunk(rc->x*chunkSize,           worldY, rc->z*chunkSize);
				testChunk(rc->x*chunkSize+chunkSize, 0,      rc->z*chunkSize);
				testChunk(rc->x*chunkSize+chunkSize, worldY, rc->z*chunkSize);
				testChunk(rc->x*chunkSize,           0,      rc->z*chunkSize+chunkSize);
				testChunk(rc->x*chunkSize,           worldY, rc->z*chunkSize+chunkSize);
				testChunk(rc->x*chunkSize+chunkSize, 0,      rc->z*chunkSize+chunkSize);
				testChunk(rc->x*chunkSize+chunkSize, worldY, rc->z*chunkSize+chunkSize);
#undef testChunk
				if (!(allL || allR || allU || allD) && vZ) {
					renderorder[nrendered] = i;
					nrendered++;
				}
			}
		}
		//sort the render order
		cmp_player = thePlayer;
		qsort(renderorder, nrendered, sizeof(int), chunk_cmp);
		for (int i=0; i<nrendered; i++) {
			RenderChunk *rc = renderchunks[renderorder[i]];
			rc->list->render();
		}
		/*GX_SetTevColorIn(GX_TEVSTAGE0,GX_CC_RASC,GX_CC_ONE,GX_CC_TEXC,GX_CC_ZERO);
		GX_SetTevAlphaIn(GX_TEVSTAGE0,GX_CA_TEXA,GX_CA_RASA,GX_CA_TEXA,GX_CC_RASA);
		GX_SetTevColorOp(GX_TEVSTAGE0,GX_TEV_ADD,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);
		GX_SetTevAlphaOp(GX_TEVSTAGE0,GX_TEV_COMP_A8_GT,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);*/

		// Liquids are in the blend pass, shift down by 1.5 texels
		GRRLIB_ObjectViewBegin();
		GRRLIB_ObjectViewTrans(-thePlayer.posX, -thePlayer.posY - 1.625f - (1.5/16.0), -thePlayer.posZ);
		GRRLIB_ObjectViewRotate(0, thePlayer.lookX, 0);
		GRRLIB_ObjectViewRotate(thePlayer.lookY, 0, 0);
		GRRLIB_ObjectViewEnd();

		for (int i=0; i<nrendered; i++) {
			RenderChunk *rc = renderchunks[renderorder[i]];
			rc->blendlist->render();
		}
	}

	void getfifousage(int *usage, int *total) {
		int _usage = 0;
		int _total = 0;

		for (int i=0; i<nRenderChunks; i++) {
			RenderChunk *rc = renderchunks[i];
			if (rc->active && rc->list) {
				_usage += rc->list->index-1;
				_usage += rc->blendlist->index-1;
				_total += rc->list->size;
				_total += rc->blendlist->size;
			}
		}
		if (usage)
			*usage = _usage;
		if (total)
			*total = _total;
	}
}
