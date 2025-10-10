#pragma once

#include <Modularis_Core/user/modules/players/Sequencer/Interpolation.h>

struct MDLRS_Continuous_key
{
	float value;
	float duration;
	enum MDLRS_Interpolation curve;
};
float MDLRS_Continuous_key_get_value(struct MDLRS_Continuous_key *self, float position);