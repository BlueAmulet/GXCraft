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

#include "gfx/font.h"
#include "gfx/girl.h"

#include "main.h"
#include "player.h"
#include "controls.h"
#include "render.h"
#include "block.h"

#include "block/stone.h"

#define chunkX 16
#define chunkZ 16

unsigned char theWorld[worldY][worldX][worldZ];

player thePlayer;

inline double to_degrees(double radians) {
    return radians*(180.0f/M_PI);
}

inline double to_radians(double degrees) {
    return (degrees*M_PI)/180.0f;
}

void generateChunk(short cX, short cZ) {
	short x,y,z;
	for (y = 0; y < worldY; y++) {
		for (x = cX * chunkX; x < cX * chunkX + chunkX; x++) {
			for (z = cZ * chunkZ; z < cZ * chunkZ + chunkZ; z++) {
				if (y < 16)
					theWorld[y][x][z] = 1;
				else
					theWorld[y][x][z] = 0;
			}
		}
	}
}

void generateWorld() {
	short x,y,z;
	for (y = 0; y < worldY; y++) {
		for (x = 0; x < worldX; x++) {
			for (z = 0; z < worldZ; z++) {
				if (y < 16)
					theWorld[y][x][z] = 1;
				else
					theWorld[y][x][z] = 0;
			}
		}
	}
}

static u8 CalculateFrameRate();

int main() {

	u32 TEXT_COLOR = 0x000000FF;

	int renderDistance = 64;

	u8 status = 0;
	u8 FPS = 0;

	memset(theWorld, 255, sizeof theWorld);

	thePlayer.posX = 256;
	thePlayer.posY = 19;
	thePlayer.posZ = 256;
	thePlayer.motionX = 0;
	thePlayer.motionY = 0;
	thePlayer.motionZ = 0;
	thePlayer.lookX = 0;
	thePlayer.lookY = 0;
	thePlayer.lookZ = 0;

    GRRLIB_Init();
    WPAD_Init();

	GRRLIB_Settings.antialias = false;

    GRRLIB_texImg *tex_girl = GRRLIB_LoadTexture(girl);
    GRRLIB_texImg *tex_font = GRRLIB_LoadTexture(font);
    GRRLIB_InitTileSet(tex_font, 16, 16, 32);

    GRRLIB_SetBackgroundColour(0x00, 0x00, 0x00, 0xFF);

    while(1) {
		switch(status) {
		case 0:
		    GRRLIB_2dMode();
			GRRLIB_Printf(152, 232, tex_font, 0xFFFFFFFF, 1, "REGISTERING BLOCKS...");
		    GRRLIB_Render();
			stone_init();
			status++;
			break;
		case 1:
		    GRRLIB_2dMode();
			GRRLIB_Printf(160, 232, tex_font, 0xFFFFFFFF, 1, "GENERATING WORLD ...");
		    GRRLIB_Render();
			generateWorld();
			status++;
			GRRLIB_SetBackgroundColour(0x87, 0xCE, 0xEB, 0xFF);
			break;
		case 2:
		    GRRLIB_2dMode();
		    WPAD_ScanPads();
		    if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
		    //if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) thePlayer.lookX += 0.3f;
		    //if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) thePlayer.lookX -= 0.3f;
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
			//GRRLIB_Camera3dSettings(0, 1.625f, -13, 0, 1, 0, thePlayer.lookX, thePlayer.lookY, thePlayer.lookZ);

		    GRRLIB_SetTexture(tex_girl,0);
		    //GRRLIB_ObjectView(0,0,cubeZ, a,a*2,a*3,1,1,1);

			int x;
			int y;
			int z;
			for (x = thePlayer.posX - renderDistance; x <= thePlayer.posX + renderDistance; x++) {
				for (z = thePlayer.posZ - renderDistance; z <= thePlayer.posZ + renderDistance; z++) {
					for (y = 14; y < 16; y++) {
						unsigned char blockID = theWorld[y][x][z];
						if (blockID != 0) {
							blockEntry entry = blockRegistry[blockID];
							GRRLIB_ObjectViewInv(x - thePlayer.posX, y - thePlayer.posY, z - thePlayer.posZ, thePlayer.lookY, thePlayer.lookX, thePlayer.lookZ, 0.5, 0.5, 0.5);
							entry.renderBlock(blockID, x, y, z);
						}
					}
				}
			}

		    // Switch To 2D Mode to display text
		    GRRLIB_2dMode();
			GRRLIB_Printf(10, 25, tex_font, TEXT_COLOR, 1, "FPS: %d", FPS);
			GRRLIB_Printf(10, 40, tex_font, TEXT_COLOR, 1, "PX: %f", thePlayer.posX);
			GRRLIB_Printf(10, 55, tex_font, TEXT_COLOR, 1, "PY: %f", thePlayer.posY);
			GRRLIB_Printf(10, 70, tex_font, TEXT_COLOR, 1, "PZ: %f", thePlayer.posZ);
			GRRLIB_Printf(10, 85, tex_font, TEXT_COLOR, 1, "LX: %f", thePlayer.lookX);
			GRRLIB_Printf(10, 100, tex_font, TEXT_COLOR, 1, "LY: %f", thePlayer.lookY);
			GRRLIB_Printf(10, 115, tex_font, TEXT_COLOR, 1, "LZ: %f", thePlayer.lookZ);

		    GRRLIB_Render();
			FPS = CalculateFrameRate();
			break;
		}
    }
    GRRLIB_FreeTexture(tex_girl);
    GRRLIB_FreeTexture(tex_font);

	// Ask blocks to cleanup
	stone_clean();

    GRRLIB_Exit();
    exit(0);
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
