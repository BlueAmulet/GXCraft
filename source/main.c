/*===========================================
    GXCraft by gamax92
    Based off of the 3D_sample2 demo
============================================*/
#include <grrlib.h>

#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>

#include "textures/font.h"

#include "main.h"
#include "player.h"
#include "controls.h"
#include "render.h"
#include "block.h"
#include "terrain.h"

#include "block/block_includes.h"

#define chunkX 16
#define chunkZ 16

#define displayListSize ((8*1024*1024)+63)

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

unsigned char theWorld[worldY][worldX][worldZ];

player thePlayer;

inline double to_degrees(double radians) {
    return radians*(180.0f/M_PI);
}

inline double to_radians(double degrees) {
    return (degrees*M_PI)/180.0f;
}

static void generateWorld() {
	generateTerrain();
	short x,y,z;
	for (x = 0; x < worldX; x++) {
		for (z = 0; z < worldZ; z++) {
			double terrainPiece = floor(((terrainData[x][z] - minY) / (maxY - minY) * 8) + 28);
			for (y = 0; y < worldY; y++) {
				if (y == 0)
					theWorld[y][x][z] = 7;
				else if (y < terrainPiece - 3)
					theWorld[y][x][z] = 1;
				else if (y < terrainPiece && y >= terrainPiece - 2)
					theWorld[y][x][z] = 3;
				else if (y == terrainPiece)
					theWorld[y][x][z] = 2;
				else if (y == terrainPiece + 1) {
					int type = rand() % 100;
					if (type == 0)
						theWorld[y][x][z] = 37;
					else if (type == 1)
						theWorld[y][x][z] = 38;
					else
						theWorld[y][x][z] = 0;
				} else
					theWorld[y][x][z] = 0;
			}
		}
	}
}

static void initializeBlocks();
static void cleanBlocks();
static u8 CalculateFrameRate();

int main() {

	time_t t;
	srand((unsigned) time(&t));

	u32 TEXT_COLOR = 0x000000FF;

	int renderDistance = 64;

	u8 status = 0;
	u8 FPS = 0;

	memset(theWorld, 255, sizeof theWorld);
	
	void *displayList = memalign(32, displayListSize);
	bool rerenderDisplayList = true;
	int dlrendersize = 0;

	thePlayer.posX = 256;
	thePlayer.posZ = 256;
	thePlayer.motionX = 0;
	thePlayer.motionY = 0;
	thePlayer.motionZ = 0;
	thePlayer.lookX = 0;
	thePlayer.lookY = 0;
	thePlayer.lookZ = 0;

	int displistX = 256;
	int displistY = 18;
	int displistZ = 256;

    GRRLIB_Init();
    WPAD_Init();

	GRRLIB_Settings.antialias = false;

    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    while (true) {
		switch(status) {
		case 0: // Register blocks
		    GRRLIB_2dMode();
			GRRLIB_Printf(152, 232, tex_font, 0xFFFFFFFF, 1, "REGISTERING BLOCKS...");
		    GRRLIB_Render();
			initializeBlocks();
			status++;
			break;
		case 1: // Generate the world
		    GRRLIB_2dMode();
			GRRLIB_Printf(160, 232, tex_font, 0xFFFFFFFF, 1, "GENERATING WORLD ...");
		    GRRLIB_Render();
			generateWorld();
			int y;
			for (y = worldY - 1; y >= 0; y--) {
				if (theWorld[y][(int)thePlayer.posX][(int)thePlayer.posZ] != 0) {
					thePlayer.posY = y;
					break;
				}
			}
			status++;
			GRRLIB_SetBackgroundColour(0x9E, 0xCE, 0xFF, 0xFF);
			break;
		case 2: // Main loop
		    GRRLIB_2dMode();
		    WPAD_ScanPads();
		    if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
			if(WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_UP) {
				thePlayer.posX += sin(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.posZ -= cos(to_radians(thePlayer.lookX)) * 0.1;
			}
			if(WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_DOWN) {
				thePlayer.posX -= sin(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.posZ += cos(to_radians(thePlayer.lookX)) * 0.1;
			}
			if(WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_LEFT) {
				thePlayer.posX -= cos(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.posZ -= sin(to_radians(thePlayer.lookX)) * 0.1;
			}
			if(WPAD_ButtonsHeld(WPAD_CHAN_0) & WPAD_BUTTON_RIGHT) {
				thePlayer.posX += cos(to_radians(thePlayer.lookX)) * 0.1;
				thePlayer.posZ += sin(to_radians(thePlayer.lookX)) * 0.1;
			}
			thePlayer.lookX += WPAD_StickX(WPAD_CHAN_0, 0) / 128.f;
			thePlayer.lookY -= WPAD_StickY(WPAD_CHAN_0, 0) / 128.f;

			// Keep values in bound
			if (thePlayer.lookX > 180)       thePlayer.lookX -= 360;
			else if (thePlayer.lookX < -180) thePlayer.lookX += 360;
			if (thePlayer.lookY > 90)        thePlayer.lookY =  90;
			else if (thePlayer.lookY < -90)  thePlayer.lookY = -90;
			if (thePlayer.lookZ > 180)       thePlayer.lookZ -= 360;
			else if (thePlayer.lookZ < -180) thePlayer.lookZ += 360;

		    GRRLIB_3dMode(0.1, 1000, 45, 1, 0);
			//GRRLIB_Camera3dSettings(thePlayer.posX, thePlayer.posY, thePlayer.posZ, 0, 1, 0, thePlayer.lookX, thePlayer.lookY, thePlayer.lookZ);

			GRRLIB_ObjectViewBegin();
			GRRLIB_ObjectViewTrans(-thePlayer.posX, -thePlayer.posY - 1.625, -thePlayer.posZ);
			GRRLIB_ObjectViewRotate(0, thePlayer.lookX, 0);
			GRRLIB_ObjectViewRotate(thePlayer.lookY, 0, 0);
			GRRLIB_ObjectViewEnd();
			
			if (abs(displistX - thePlayer.posX) + abs(displistY - thePlayer.posY) + abs(displistZ - thePlayer.posZ) > 8) {
				rerenderDisplayList = true;
				displistX = thePlayer.posX;
				displistY = thePlayer.posY;
				displistZ = thePlayer.posZ;
			}
			
			//GRRLIB clears the vertex formats on mode switch
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_CLR_RGB, GX_RGBA4, 0);
			GX_SetVtxAttrFmt(GX_VTXFMT1, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);

			if (rerenderDisplayList)
			{
				rerenderDisplayList = false;
				lastTex = NULL;
				GX_BeginDispList(displayList, displayListSize);
				int x;
				int y;
				int z;
				for (y = worldY - 1; y >= 0; y--) {
					for (x = max(thePlayer.posX - renderDistance,0); x <= min(worldX-1,thePlayer.posX + renderDistance); x++) {
						for (z = max(thePlayer.posZ - renderDistance,0); z <= min(worldZ-1,thePlayer.posZ + renderDistance); z++) {
							unsigned char blockID = theWorld[y][x][z];
							if (blockID != 0) {
								blockEntry entry = blockRegistry[blockID];
								entry.renderBlock(x, y, z, 0);
							}
						}
					}
				}
				
				for (y = worldY - 1; y >= 0; y--) {
					for (x = max(thePlayer.posX - renderDistance,0); x <= min(worldX-1,thePlayer.posX + renderDistance); x++) {
						for (z = max(thePlayer.posZ - renderDistance,0); z <= min(worldZ-1,thePlayer.posZ + renderDistance); z++) {
							unsigned char blockID = theWorld[y][x][z];
							if (blockID != 0) {
								blockEntry entry = blockRegistry[blockID];
								entry.renderBlock(x, y, z, 1);
							}
						}
					}
				}
				dlrendersize = GX_EndDispList();
			}
			GX_CallDispList(displayList, dlrendersize);

		    // Draw 2D elements
		    GRRLIB_2dMode();
			GRRLIB_Printf(10,  25, tex_font, TEXT_COLOR, 1, "FPS: %d", FPS);
			GRRLIB_Printf(10,  40, tex_font, TEXT_COLOR, 1, "PX:% 7.2f", thePlayer.posX);
			GRRLIB_Printf(10,  55, tex_font, TEXT_COLOR, 1, "PY:% 7.2f", thePlayer.posY);
			GRRLIB_Printf(10,  70, tex_font, TEXT_COLOR, 1, "PZ:% 7.2f", thePlayer.posZ);
			GRRLIB_Printf(10,  85, tex_font, TEXT_COLOR, 1, "LX:% 7.2f", thePlayer.lookX);
			GRRLIB_Printf(10, 100, tex_font, TEXT_COLOR, 1, "LY:% 7.2f", thePlayer.lookY);
			GRRLIB_Printf(10, 115, tex_font, TEXT_COLOR, 1, "LZ:% 7.2f", thePlayer.lookZ);
			GRRLIB_Printf(10, 130, tex_font, TEXT_COLOR, 1, "DLSIZE: %i/%i (%i%%)", dlrendersize, displayListSize, dlrendersize*100/displayListSize);

		    GRRLIB_Render();
			FPS = CalculateFrameRate();
			break;
		}
    }
    GRRLIB_FreeTexture(tex_font);

	// Ask blocks to cleanup
	cleanBlocks();

    GRRLIB_Exit();
    exit(0);
}

static void initializeBlocks() {
	bedrock_init();
	brick_init();
	cloth_aqua_init();
	cloth_black_init();
	cloth_blue_init();
	cloth_cyan_init();
	cloth_gray_init();
	cloth_green_init();
	cloth_indigo_init();
	cloth_lime_init();
	cloth_magenta_init();
	cloth_orange_init();
	cloth_pink_init();
	cloth_purple_init();
	cloth_red_init();
	cloth_violet_init();
	cloth_white_init();
	cloth_yellow_init();
	cobble_init();
	dirt_init();
	flower_init();
	glass_init();
	gold_init();
	grass_init();
	gravel_init();
	iron_init();
	lava_init();
	leaves_init();
	log_init();
	mossy_init();
	mushroom_init();
	obsidna_init();
	ore_coal_init();
	ore_gold_init();
	ore_iron_init();
	redshroom_init();
	rose_init();
	sand_init();
	sapling_init();
	shelf_init();
	slab_init();
	sponge_init();
	stone_init();
	tnt_init();
	water_init();
	wood_init();
}

static void cleanBlocks() {
	bedrock_clean();
	brick_clean();
	cloth_aqua_clean();
	cloth_black_clean();
	cloth_blue_clean();
	cloth_cyan_clean();
	cloth_gray_clean();
	cloth_green_clean();
	cloth_indigo_clean();
	cloth_lime_clean();
	cloth_magenta_clean();
	cloth_orange_clean();
	cloth_pink_clean();
	cloth_purple_clean();
	cloth_red_clean();
	cloth_violet_clean();
	cloth_white_clean();
	cloth_yellow_clean();
	cobble_clean();
	dirt_clean();
	flower_clean();
	glass_clean();
	gold_clean();
	grass_clean();
	gravel_clean();
	iron_clean();
	lava_clean();
	leaves_clean();
	log_clean();
	mossy_clean();
	mushroom_clean();
	obsidna_clean();
	ore_coal_clean();
	ore_gold_clean();
	ore_iron_clean();
	redshroom_clean();
	rose_clean();
	sand_clean();
	sapling_clean();
	shelf_clean();
	slab_clean();
	sponge_clean();
	stone_clean();
	tnt_clean();
	water_clean();
	wood_clean();
}

static u8 CalculateFrameRate() {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}
