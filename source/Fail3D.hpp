#pragma once

#include <gccore.h>

namespace Fail3D {
	void init(double);
	void translatePoint(guVector*, guVector*);
	void rotatePoint(guVector*, guVector*);
	void calculatePointPosition(guVector*);
};
