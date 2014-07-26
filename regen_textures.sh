#!/bin/sh

cd source/textures/
rm font.c
rm font.h
rm block_*.c
rm block_*.h
for f in *.png; do echo -n "textures/$f - "; raw2c $f; done
cd ../../

