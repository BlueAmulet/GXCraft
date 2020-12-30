#pragma once

#include <gccore.h>

namespace Fail3D {
	void init(float);
	void translatePoint(guVector*, guVector*);
	void rotatePoint(guVector*, guVector*);
	void calculatePointPosition(guVector*);
}
