#include <cmath>

#include "Utils.hpp"
#include "Main.hpp"

template <typename T> static int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

// We want specifically this type of modulus, fmod returns different values for negatives.
template <typename T> static T mod(T x, T y) {
	return (x - floor(x/y)*y);
}

template <typename T> static T intbound(T s, T ds) {
	if (ds == T(0)) // Protect against negative zero
		ds = T(0);
	if (ds < T(0)) {
		s=mod(-s, T(1));
		if (s == T(0))
			return T(0);
		else
			return (s - 1) / ds;
	} else {
		s=mod(s, T(1));
		return (T(1) - s) / ds;
	}
}

namespace Utils {
	bool voxelCollisionLine(double x1, double y1, double z1, double x2, double y2, double z2, int *cx, int *cy, int *cz, int *fx, int *fy, int *fz) {
		return voxelCollisionRay(x1, y1, z1, x2-x1, y2-y1, z2-z1, cx, cy, cz, fx, fy, fz);
	}

	bool voxelCollisionRay(double x, double y, double z, double dx, double dy, double dz, int *cx, int *cy, int *cz, int *fx, int *fy, int *fz) {
		int xPos = floor(x);
		int yPos = floor(y);
		int zPos = floor(z);

		double range = sqrt(pow(dx, 2)+ pow(dy, 2) + pow(dz, 2));
		// Normalize
		dx /= range;
		dy /= range;
		dz /= range;

		int stepX = signum(dx);
		int stepY = signum(dy);
		int stepZ = signum(dz);
		double maxX = intbound(x, dx), maxY = intbound(y, dy), maxZ = intbound(z, dz);
		double deltaX = (double)stepX / dx, deltaY = (double)stepY / dy, deltaZ = (double)stepZ / dz;
		int faceX = 0;
		int faceY = 0;
		int faceZ = 0;

		do {
			uint8_t block = theWorld->getBlock(xPos, yPos, zPos);
			if (block != 0 && block != 8 && block != 10) {
				if (cx) *cx = xPos;
				if (cy) *cy = yPos;
				if (cz) *cz = zPos;
				if (fx) *fx = faceX;
				if (fy) *fy = faceY;
				if (fz) *fz = faceZ;
				return true;
			}

			if (maxX < maxY) {
				if (maxX < maxZ) {
					if (maxX > range)
						break;
					xPos += stepX;
					maxX += deltaX;
					faceX = -stepX;
					faceY = 0;
					faceZ = 0;
				} else {
					if (maxZ > range)
						break;
					zPos += stepZ;
					maxZ += deltaZ;
					faceX = 0;
					faceY = 0;
					faceZ = -stepZ;
				}
			} else {
				if (maxY < maxZ) {
					if (maxY > range)
						break;
					yPos += stepY;
					maxY += deltaY;
					faceX = 0;
					faceY = -stepY;
					faceZ = 0;
				} else {
					if (maxZ > range)
						break;
					zPos += stepZ;
					maxZ += deltaZ;
					faceX = 0;
					faceY = 0;
					faceZ = -stepZ;
				}
			}
		} while (true);
		return false;
	}
}
