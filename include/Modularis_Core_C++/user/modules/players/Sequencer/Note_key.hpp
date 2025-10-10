#pragma once

#include <Modularis_Core_C++/user/modules/players/Sequencer/Continuous_key.hpp>

namespace MDLRS
{
	struct Note_key: Continuous_key
	{
		bool pressed;

		float get_value(float position);
	};
}