#!/bin/sh

cd source/textures/
rm -f *.c
rm -f *.h
for f in *.png; do echo -n "textures/$f - "; raw2c $f; done
cd ../gfx/
rm -f *.c
rm -f *.h
for f in *.png; do echo -n "gfx/$f - "; raw2c $f; done
cd ../../

