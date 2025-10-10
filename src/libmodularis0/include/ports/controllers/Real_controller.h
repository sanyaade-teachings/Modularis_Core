#pragma once

#include <Modularis_Core/system/interfaces/ports/controllers/Real_controller.h>
#include <system/ports/Port.h>

struct MDLRS_Real_controller
{
	struct MDLRS_Port p;

	float value;
};