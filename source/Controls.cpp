#include <cmath>

#include <gctypes.h>
#include <wiiuse/wpad.h>

#include "Controls.hpp"

s8 WPAD_StickX(u8 chan, u8 right) {
	float mag = 0.0f;
	float ang = 0.0f;
	WPADData *data = WPAD_Data(chan);

	switch (data->exp.type) {
	case WPAD_EXP_NUNCHUK:
	case WPAD_EXP_GUITARHERO3:
		if (right == 0) {
			mag = data->exp.nunchuk.js.mag;
			ang = data->exp.nunchuk.js.ang;
		}
		break;

	case WPAD_EXP_CLASSIC:
		if (right == 0) {
			mag = data->exp.classic.ljs.mag;
			ang = data->exp.classic.ljs.ang;
		} else {
			mag = data->exp.classic.rjs.mag;
			ang = data->exp.classic.rjs.ang;
		}
		break;

	default:
		break;
	}

	/* calculate X value (angle need to be converted into radian) */
	if (mag > 1.0f)
		mag = 1.0f;
	else if (mag < -1.0f)
		mag = -1.0f;
	else if (mag < 0.1f && mag > -0.1f)
		mag = 0.0f;
	double val = mag * sin(M_PI * ang / 180.0f);

	return static_cast<s8>(val * 128.0f);
}

s8 WPAD_StickY(u8 chan, u8 right) {
	float mag = 0.0f;
	float ang = 0.0f;
	WPADData *data = WPAD_Data(chan);

	switch (data->exp.type) {
	case WPAD_EXP_NUNCHUK:
	case WPAD_EXP_GUITARHERO3:
		if (right == 0) {
			mag = data->exp.nunchuk.js.mag;
			ang = data->exp.nunchuk.js.ang;
		}
		break;

	case WPAD_EXP_CLASSIC:
		if (right == 0) {
			mag = data->exp.classic.ljs.mag;
			ang = data->exp.classic.ljs.ang;
		} else {
			mag = data->exp.classic.rjs.mag;
			ang = data->exp.classic.rjs.ang;
		}
		break;

	default:
		break;
	}

	/* calculate X value (angle need to be converted into radian) */
	if (mag > 1.0f)
		mag = 1.0f;
	else if (mag < -1.0f)
		mag = -1.0f;
	else if (mag < 0.1f && mag > -0.1f)
		mag = 0.0f;
	double val = mag * cos(M_PI * ang / 180.0f);

	return static_cast<s8>(val * 128.0f);
}
