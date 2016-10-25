#include <grrlib.h>
#include <cmath>
#include <cstdlib>
#include <algorithm>

#include "chunked_render.hpp"
#include "netcat_logger.hpp"
#include "block.hpp"
#include "render.hpp"
#include "fail3d.hpp"
#include "main.hpp"

#define chunked_isoob(rc,rcd,px,pz) (rc->x > px+rcd) || (rc->x < px-rcd) || (rc->z > pz+rcd) || (rc->z < pz-rcd)

renderchunk* Chunked::renderchunks[nRenderChunks];
int renderorder[nRenderChunks];

void Chunked::init() {
	//initialize all the renderchunks
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = static_cast<renderchunk*>(malloc(sizeof(renderchunk)));
		rc->active = false;
		rc->update = false;
		rc->list = NULL;
		renderchunks[i] = rc;
	}
}

void Chunked::deallocall() {
	int i;
	//dealloc all the renderchunks
	for (i=0; i<nRenderChunks; i++) {
		renderchunks[i]->active = false;
	}
}

int Chunked::getchunkfromchunkpos(unsigned short x, unsigned short z) {
	//first try to find an existing renderchunk with the coords
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
		if (rc->active && rc->x == x && rc->z == z) {
			Netcat::log("found active chunk with same x and z\n");
			return i;
		}
	}
	//else, find an inactive chunk
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
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

void Chunked::markchunkforupdate(unsigned short x, unsigned short z) {
	//try to find an existing renderchunk with the coords
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
		if (rc->active && rc->x == x && rc->z == z) {
			Netcat::log("marking active chunk for render update\n");
			rc->update = true;
			return;
		}
	}
	Netcat::log("no chunk to mark for render update!\n");
}

void Chunked::rerenderChunkUpdates() {
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
		if (rc->active && rc->update) {
			Chunked::rerenderChunk(rc->x, rc->z, true);
		}
	}
}

inline void Chunked::rerenderChunk(signed short cx, signed short cz, bool force) {
	if (cx < 0 || cz < 0 || cx >= worldX/chunkSize || cz >= worldZ/chunkSize)
		return;
	renderchunk *rc = renderchunks[Chunked::getchunkfromchunkpos(cx,cz)];
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
					u8 blockID = theWorld[y][x][z];
					if (blockID != 0) {
						blockEntry entry = blockRegistry[blockID];
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
					u8 blockID = theWorld[y][x][z];
					if (blockID != 0) {
						blockEntry entry = blockRegistry[blockID];
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

void Chunked::refresh(int renderDistance, player thePlayer) {
	//convert the player's position to chunk position
	signed short px, pz;
	px = thePlayer.posX/chunkSize;
	pz = thePlayer.posZ/chunkSize;
	int rcd = renderDistance/chunkSize;
	//remove the old chunks that are now out of range
	int nactive = 0;
	int nremoved = 0;
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
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
			Chunked::rerenderChunk(cx,cz,false);
		}
	}
}

static player cmp_player;
int Chunked::chunk_cmp(const void *a, const void *b) {
	renderchunk *ca = Chunked::renderchunks[*((const int *)a)];
	renderchunk *cb = Chunked::renderchunks[*((const int *)b)];
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

void Chunked::render(player thePlayer) {
	guVector center = {thePlayer.posX, -thePlayer.posY - 1.625f, -thePlayer.posZ};
	guVector camera = {thePlayer.lookY, thePlayer.lookX, thePlayer.lookZ};
	guVector polygon;

	DisplayList::start();
	int i;
	int nrendered = 0;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
		if (rc->active) {
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
	for (i=0; i<nrendered; i++) {
		renderchunk *rc = renderchunks[renderorder[i]];
		rc->list->render();
	}
	/*GX_SetTevColorIn(GX_TEVSTAGE0,GX_CC_RASC,GX_CC_ONE,GX_CC_TEXC,GX_CC_ZERO);
	GX_SetTevAlphaIn(GX_TEVSTAGE0,GX_CA_TEXA,GX_CA_RASA,GX_CA_TEXA,GX_CC_RASA);
	GX_SetTevColorOp(GX_TEVSTAGE0,GX_TEV_ADD,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);
	GX_SetTevAlphaOp(GX_TEVSTAGE0,GX_TEV_COMP_A8_GT,GX_TB_ZERO,GX_CS_SCALE_1,GX_TRUE,GX_TEVPREV);*/
	for (i=0; i<nrendered; i++) {
		renderchunk *rc = renderchunks[renderorder[i]];
		rc->blendlist->render();
	}
}

int Chunked::getfifousage() {
	int usage = 0;
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
		if (rc->active) {
			usage += rc->list->index-1;
			usage += rc->blendlist->index-1;
		}
	}
	return usage;
}

int Chunked::getfifototal() {
	int usage = 0;
	int i;
	for (i=0; i<nRenderChunks; i++) {
		renderchunk *rc = renderchunks[i];
		if (rc->active) {
			usage += rc->list->size;
			usage += rc->blendlist->size;
		}
	}
	return usage;
}