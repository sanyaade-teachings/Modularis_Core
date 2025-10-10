#pragma once

#include <Modularis_Core/system/interfaces/ports/controllers/Integer_controller.h>
#include <system/ports/Port.h>

#include <stdint.h>

struct MDLRS_Integer_controller
{
	struct MDLRS_Port p;

	int32_t value;
};