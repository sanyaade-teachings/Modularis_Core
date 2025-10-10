#pragma once

#include <Modularis_Core/user/modules/players/Sequencer/Continuous_key.h>

#include <stdbool.h>

struct MDLRS_Note_key
{
	struct MDLRS_Continuous_key p;

	bool pressed;
};
float MDLRS_Note_key_get_value(struct MDLRS_Note_key *self, float position);