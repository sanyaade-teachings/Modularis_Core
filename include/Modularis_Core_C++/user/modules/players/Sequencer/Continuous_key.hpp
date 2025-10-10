#pragma once

#include <Modularis_Core_C++/user/modules/players/Sequencer/Interpolation.hpp>

namespace MDLRS
{
	struct Continuous_key
	{
		float value;
		float duration;
		Interpolation curve;

		float get_value(float position);
	};
}