#!/bin/sh

cd source/textures/
rm *.c *.h
for f in *.png; do echo -n "textures/$f - "; raw2c $f; done
cd inv_blocks/
rm *.c *.h *.cpp *.hpp
for f in *.png; do echo -n "textures/inv_blocks/$f - "; raw2c $f; done
echo -e "#include <grrlib.h>\n#include \"../../main.hpp\"\n#include \"block_icons.hpp\"\n\nextern \"C\" {" > block_icons.cpp
ls bi_*.h | sed 's/^bi_//' | sed 's/\.h$//' | sort -n | awk '{ printf( "#include \"bi_%s.h\"\n", $1 ); }' >> block_icons.cpp
echo -e "}\n\nvoid load_bi() {" >> block_icons.cpp
ls bi_*.h | sed 's/^bi_//' | sed 's/\.h$//' | sort -n | awk '{ printf( "\ttex_blockicons[%2s] = GRRLIB_LoadTexture(bi_%s);\n", $1, $1 ); }' >> block_icons.cpp
echo -e "}" >> block_icons.cpp
echo -e "#pragma once\nextern void load_bi();" > block_icons.hpp
cd ../../../
