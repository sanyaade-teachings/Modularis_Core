#pragma once

#include <Modularis_Core/system/interfaces/ports/Sound.h>
#include <system/ports/Port.h>

#include <Modularis_Core/system/types/Sound_value.h>

struct MDLRS_Sound
{
	struct MDLRS_Port p;

	MDLRS_Sound_value frame;
};