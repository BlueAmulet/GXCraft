typedef struct blockEntry_s {
	void (*renderBlock)(unsigned char blockID, int xPos, int yPos, int zPos, unsigned char pass);
} blockEntry;

extern blockEntry blockRegistry[256];

extern void registerBlock(unsigned char id, blockEntry entry);
