#include <cmath>
#include <grrlib.h>

#include "Fail3D.hpp"

double Fail3D::focalLength;
double Fail3D::vanishingPointX;
double Fail3D::vanishingPointY;

void Fail3D::init(double fl) {
	focalLength = fl;
	vanishingPointX = rmode->fbWidth/2;
	vanishingPointY = rmode->efbHeight/2;
}

void Fail3D::translatePoint(guVector *polygon, guVector *center) {
	polygon->x -= center->x;
	polygon->y -= center->y;
	polygon->z -= center->z;
}

void Fail3D::rotateCoords(double *a, double *b, double angle) {
	double cosV = cos(angle);
	double sinV = sin(angle);
	double a1 = (*a * cosV) - (*b * sinV);
	double b1 = (*b * cosV) + (*a * sinV);
	*a = a1;
	*b = b1;
}

void Fail3D::rotatePoint(guVector *polygon, guVector *camera) {
	double x = polygon->x;
	double y = polygon->y;
	double z = polygon->z;

	rotateCoords(&x, &z, DegToRad(camera->y));
	rotateCoords(&y, &z, DegToRad(camera->x));

	polygon->x = x;
	polygon->y = y;
	polygon->z = z;
}

void Fail3D::calculatePointPosition(guVector *polygon) {
	double scale = fabs(focalLength/polygon->z);
	polygon->x = vanishingPointX + polygon->x * scale;
	polygon->y = vanishingPointY + polygon->y * scale;
}