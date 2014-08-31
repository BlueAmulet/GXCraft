#!/bin/sh

cd source/textures/
rm *.c
rm *.h
for f in *.png; do echo -n "textures/$f - "; raw2c $f; done
cd inv_blocks/
rm *.c
rm *.h
for f in *.png; do echo -n "textures/inv_blocks/$f - "; raw2c $f; done
echo "#include \"../../main.h\"\n#include \"block_icons.h\"\n" > block_icons.c
ls bi_*.h | sed 's/^bi_//' | sed 's/\.h$//' | sort -n | awk '{ printf( "#include \"bi_%s.h\"\n", $1 ); }' >> block_icons.c
echo "\nvoid load_bi() {" >> block_icons.c
ls bi_*.h | sed 's/^bi_//' | sed 's/\.h$//' | sort -n | awk '{ printf( "\ttex_blockicons[%2s] = GRRLIB_LoadTexture(bi_%s);\n", $1, $1 ); }' >> block_icons.c
echo "}" >> block_icons.c
echo "extern void load_bi();" > block_icons.h
cd ../../../

