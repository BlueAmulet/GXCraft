#pragma once
void fail3d_init(double fl);
void fail3d_translatePoint(guVector *polygon, guVector *center);
void fail3d_rotatePoint(guVector *polygon, guVector *camera);
void fail3d_calculatePointPosition(guVector *polygon);
