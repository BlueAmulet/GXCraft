#!/bin/sh

cd source/textures/
rm *.c
rm *.h
for f in *.png; do echo -n "textures/$f - "; raw2c $f; done
cd ../../

