#pragma once
class Fail3D {
	private:
		static double focalLength;
		static double vanishingPointX;
		static double vanishingPointY;

		static void rotateCoords(double*, double*, double);
	public:
		static void init(double);
		static void translatePoint(guVector*, guVector*);
		static void rotatePoint(guVector*, guVector*);
		static void calculatePointPosition(guVector*);
};
