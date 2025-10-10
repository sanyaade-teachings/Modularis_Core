#pragma once

#include <Modularis_Core/ports/controllers/ADSR.h>
#include <stdint.h>

struct Sample
{
	struct Sample *previous, *next;
	enum MDLRS_ADSR_state state;
	float speed, velocity, frame;
	uint32_t time; //It's needed for ADSR.
};